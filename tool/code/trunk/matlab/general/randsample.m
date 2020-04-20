function randsamples = randsample(samplength,low,high)

url = java.net.URL(['http://www.random.org/integers/?num=' num2str(samplength) '&min=' num2str(low) '&max=' num2str(high) '&col=1&base=10&format=plain&rnd=new']);
is = openStream(url);
isr = java.io.InputStreamReader(is);
br = java.io.BufferedReader(isr);
list=zeros(samplength,1);
for k=1:nnov,
    list(k)=readLine(br);
end

