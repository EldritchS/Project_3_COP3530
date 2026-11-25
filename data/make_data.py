import numpy as np
from scipy.spatial import cKDTree
from scipy.sparse import coo_matrix
from scipy.sparse.csgraph import connected_components
R=6371000.0
def hav(la1,lo1,la2,lo2):
    t=np.pi/180; a=np.sin((la2-la1)*t/2)**2+np.cos(la1*t)*np.cos(la2*t)*np.sin((lo2-lo1)*t/2)**2
    return 2*R*np.arcsin(np.minimum(1,np.sqrt(a)))
def write(name,lat,lon,U,V,desc):
    n=len(lat)
    w=np.ceil(hav(lat[U],lon[U],lat[V],lon[V])*np.random.uniform(1.0,1.25,len(U))).astype(np.int64)+1
    with open(name+'.co','w') as f:
        f.write(f"c {desc}\nc SYNTHETIC data generated for testing; not a real road network\np aux sp co {n}\n")
        f.writelines(f"v {i+1} {int(round(lon[i]*1e6))} {int(round(lat[i]*1e6))}\n" for i in range(n))
    with open(name+'.gr','w') as f:
        f.write(f"c {desc}\nc SYNTHETIC data; lengths in meters\np sp {n} {len(U)}\n")
        f.writelines(f"a {u+1} {v+1} {x}\n" for u,v,x in zip(U,V,w))
    print(name,n,'vertices',len(U),'edges')
def biggest_component_check(n,U,V):
    m=coo_matrix((np.ones(len(U)),(U,V)),shape=(n,n))
    k,lab=connected_components(m,directed=True,connection='strong')
    return np.bincount(lab).max()/n

# ---------- Dataset 1: city street grid ----------
rng=np.random.default_rng(1); np.random.seed(1)
N=400                                   # 400 x 400 = 160,000 intersections
i,j=np.meshgrid(np.arange(N),np.arange(N),indexing='ij'); i=i.ravel(); j=j.ravel()
lat=28.30+i*0.00090+rng.normal(0,0.00012,N*N)+0.002*np.sin(j/40)   # ~100 m blocks, wavy streets
lon=-81.60+j*0.00102+rng.normal(0,0.00012,N*N)
idx=lambda a,b:a*N+b
U=[];V=[]
for di,dj in((0,1),(1,0)):
    ok=(i+di<N)&(j+dj<N); a=idx(i[ok],j[ok]); b=idx(i[ok]+di,j[ok]+dj)
    keep=rng.random(len(a))>0.08                       # 8% of blocks have no street (parks, lakes)
    a=a[keep];b=b[keep]
    oneway=rng.random(len(a))<0.15                     # 15% one-way streets
    flip=rng.random(len(a))<0.5
    U+= [np.where(oneway&flip,b,a)]; V+=[np.where(oneway&flip,a,b)]
    two=~oneway; U+=[b[two]]; V+=[a[two]]
# a few diagonal avenues
for k in range(-N,N,60):
    a=[idx(x,x+k) for x in range(N) if 0<=x+k<N-1 and x<N-1]
    b=[idx(x+1,x+k+1) for x in range(N) if 0<=x+k<N-1 and x<N-1]
    U+=[np.array(a),np.array(b)]; V+=[np.array(b),np.array(a)]
U=np.concatenate(U).astype(np.int64); V=np.concatenate(V).astype(np.int64)
print('city strongly-connected share:',round(biggest_component_check(N*N,U,V),3))
write('synthetic_city',lat,lon,U,V,'Synthetic city: 400x400 street grid, removed blocks, one-way streets, diagonal avenues')

# ---------- Dataset 2: regional road network (towns + rural roads + highways) ----------
rng=np.random.default_rng(2); np.random.seed(2)
towns=60
c_lat=rng.uniform(26.0,30.5,towns); c_lon=rng.uniform(-84.5,-80.3,towns)
sizes=rng.integers(400,1800,towns)
lat=[];lon=[]
for k in range(towns):
    s=sizes[k]; r=0.01+0.00002*s
    lat.append(c_lat[k]+rng.normal(0,r,s)); lon.append(c_lon[k]+rng.normal(0,r,s))
rural=130000-sizes.sum()
lat.append(rng.uniform(26.0,30.5,rural)); lon.append(rng.uniform(-84.5,-80.3,rural))
lat=np.concatenate(lat); lon=np.concatenate(lon); n=len(lat)
xy=np.c_[lat,lon*np.cos(np.radians(28))]
tree=cKDTree(xy)
_,nb=tree.query(xy,k=4)                                   # connect to 3 nearest neighbours
a=np.repeat(np.arange(n),3); b=nb[:,1:].ravel()
pairs=np.unique(np.sort(np.c_[a,b],axis=1),axis=0)
# highways: link each town hub to its 3 nearest towns
hub=np.array([tree.query(xy[[k]] if False else np.array([[c_lat[k],c_lon[k]*np.cos(np.radians(28))]]))[1][0] for k in range(towns)])
tt=cKDTree(xy[hub]); _,tn=tt.query(xy[hub],k=4)
hw=np.array([(hub[x],hub[y]) for x in range(towns) for y in tn[x,1:]])
pairs=np.unique(np.sort(np.r_[pairs,hw],axis=1),axis=0)
# connect any leftover pieces to the main network
m=coo_matrix((np.ones(len(pairs)),(pairs[:,0],pairs[:,1])),shape=(n,n))
k,lab=connected_components(m,directed=False)
main=np.bincount(lab).argmax(); mt=cKDTree(xy[lab==main]); mid=np.where(lab==main)[0]
extra=[]
for c in range(k):
    if c==main: continue
    mem=np.where(lab==c)[0][0]; _,q=mt.query(xy[mem]); extra.append((mem,mid[q]))
if extra: pairs=np.r_[pairs,np.array(extra)]
oneway=rng.random(len(pairs))<0.05                          # a few one-way roads (ramps)
U=np.r_[pairs[:,0],pairs[~oneway,1]]; V=np.r_[pairs[:,1],pairs[~oneway,0]]
print('region strongly-connected share:',round(biggest_component_check(n,U,V),3))
write('synthetic_region',lat,lon,U.astype(np.int64),V.astype(np.int64),'Synthetic region: 60 towns + rural roads + inter-town highways')
