def knapsack (p,v,cmax): 
    n=len(p)
    opt=[[0]*(cmax + 1) for _ in range (n+1)]
    #print(opt)
    sel=[[False]*(cmax +1) for _ in range (n+1)]
    #print(sel)

    for cap in range(p[0], cmax+1) : 
        opt[0][cap] = v[0]
        #print(opt)
        sel[0][cap] = True
        #print(sel)

    for i in range (1,n): 
        for cap in range (cmax+1) :
            if cap >= p[i] and opt[i-1][cap-p[i]]+v[i] > opt[i-1][cap] :
                opt[i][cap]= opt[i-1][cap-p[i]]+v[i]
                sel[i][cap]= True
                #print(opt)
            else: 
                opt[i][cap]=opt[i-1][cap]
                sel[i][cap]= False
                #print(opt)

    cap = cmax
    solution = []
    for i in range(n-1, -1 ,-1) :
        if sel[i][cap]:
            solution.append(i)
            cap -= p[i]
           

    return(opt[n-1][cmax],solution)

p=(5,4,1,3)
v=(12,8,2,5)
cmax=11

knapsacks = knapsack(p,v,cmax)
print(knapsacks)
