import sys

def main():
    sez = sys.argv[1].split("\\n")
    ##niz = "START"
    ##niz += str(sez)+"\n\n"
    ##slo = dict()
    sloSum = dict()
    
    start=False
    for z in sez[:-1]:
        ##niz+=str(z)+"n\n"
        if "Parameter" in z:
            ##niz+="PARAM\n"
            kljuc = int(z.split(":")[1].strip())
            ##slo[kljuc]=set()
            sloSum[kljuc]=0
            start = True
        elif start:
            ##niz+="START\n"
            ##slo[k].add(tuple(z.split(";")))
            try:
                sloSum[kljuc]+=int(z.split(";")[1])
            except:
                int(z+str(sez))
    
    ##print(sloSum,sez,sez[:-1])
    m = min(sloSum.items(),key=lambda x: x[1])
    for k in sloSum:
        print(str(k) + ',' + str(sloSum[k]))
    print(m[0])
    ##print("Najboljsi:",m[0],"Cas",m[1])
    ##niz+="REZ:"
    ##niz+=str(m[0])
    ##print(niz)

if __name__ == "__main__":
    main()
