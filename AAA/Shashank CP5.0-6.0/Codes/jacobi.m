m=100;
T=zeros(21,41);
To=zeros(21,41);
T(1,:)=T(1,:)+100;
n=1;
k=0;
To
while k==0
    To=T;
    k=1;
    for j=2:20
        for i=2:40
            T(j,i)=(To(j+1,i)+To(j-1,i)+To(j,i-1)+To(j,i+1))/4;
            if abs(T(j,i)-To(j,i))>(10^-5)
                k=0;
            end
        end
    end
    n=n+1;
end
n
%To
%T
x=0:0.05:2;
y=0:0.05:1;
surface(x,y,T);
shading interp;