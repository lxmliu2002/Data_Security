import random
#快速幂计算a^b%p
def quickpower(a,b,p):
    a=a%p
    ans=1
    while b!=0:
        if b&1:
            ans=(ans*a)%p
        b>>=1
        a=(a*a)%p
    return ans
#构建多项式：x0为常数项系数，T为最高次项次数，p为模数,fname为多项式名
def get_polynomial(x0,T,p,fname):
    f=[]
    f.append(x0)
    for i in range(0,T):
        f.append(random.randrange(0,p))
    #输出多项式
    f_print='f'+fname+'='+str(f[0])
    for i in range(1,T+1):
        f_print+='+'+str(f[i])+'x^'+str(i)
    print(f_print)
    return f
#计算多项式值
def count_polynomial(f,x,p):
    ans=f[0]
    for i in range(1,len(f)):
        ans=(ans+f[i]*quickpower(x,i,p))%p
    return ans
#重构函数f并返回f(0)
def restructure_polynomial(x,fx,t,p):
    ans=0
    #利用多项式插值法计算出x=0时多项式的值
    for i in range(0,t):
        fx[i]=fx[i]%p
        fxi=1
        #在模p下，(a/b)%p=(a*c)%p，其中c为b在模p下的逆元，c=b^(p-2)%p
        for j in range(0,t):
            if j !=i:
               fxi=(-1*fxi*x[j]*quickpower(x[i]-x[j],p-2,p))%p
        fxi=(fxi*fx[i])%p
        ans=(ans+fxi)%p
    return ans
if __name__ == '__main__':
#设置模数p
    p=1000000007
    print(f'模数p：{p}')
#输入门限(t,n)
    n=int(input("请输入参与者的个数n:"))
    t=int(input("请输入需要几个人参与者才可以恢复秘密:"))
#三个投票方将自己的秘密值s[0],s[1],s[2]分别秘密共享给n个参与方
    s=[]#三个投票方的秘密值
    f=[]#三个秘密值对应的多项式
    shares_x=[]#三个秘密值对应的秘密份额
    #选择n个互不相同的随机值
    for i in range(0,n):
        temp=random.randrange(0,p)
        while temp in shares_x:
            temp=random.randrange(0,p)
        shares_x.append(temp)
    shares_y=[]
    for i in range(0,3):
        s.append(int(input(f'第{i+1}个投票方输入自己的投票值：')))
        #输出多项式及秘密份额
        print(f'第{i+1}个投票方的投票值的多项式及秘密份额：')
        f.append(get_polynomial(s[i],t-1,p,str(i+1)))
        temp=[]
        for j in range(0,n):
            temp.append(count_polynomial(f[i],shares_x[j],p))
            print(f'({shares_x[j]},{temp[j]})')
        shares_y.append(temp)
#在n个参与者中任选t个人重构求和之后的值
    #任意选取t个人
    Party=[]
    for i in range(0,t):
        temp=random.randint(0,n-1)
        while temp in Party:
            temp=random.randint(0,n-1)
        Party.append(temp)
    print('参与重构秘密的参与者:',Party)
    #t个参与方分别将自己手中s[0]、s[1]和s[2]的秘密份额相加，得到s[0]+s[1]+s[2]的秘密份额
    shares_s123_x=[]
    shares_s123_y=[]
    for i in range(0,t):
        shares_s123_x.append(shares_x[Party[i]])
        temp=0
        for j in range(0,3):
            temp+=shares_y[j][Party[i]]
        shares_s123_y.append(temp)
    s123=restructure_polynomial(shares_s123_x,shares_s123_y,t,p)
    print(f'得票结果为：{s123}')