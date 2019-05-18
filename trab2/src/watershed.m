img = imread('./img/img_cells.jpg');
%# normalize to 0...1
imgN = double(img-min(img(:)))/(max(img(:)-min(img(:))));
th1=graythresh(imgN);
th2 = graythresh(imgN(imgN>th1));

cellMsk = imgN>th1;
nucMsk = imgN>th2;

figure,imshow(cellMsk+nucMsk,[])

[xx,yy]=ndgrid(-5:5,-5:5);
gf = exp((-xx.^2-yy.^2)/20);
filtImg = conv2(imgN,gf,'same');

figure,imshow(filtImg,[])

filtImgM = imimposemin(-filtImg,nucMsk);

ws = watershed(filtImgM);
ws(~cellMsk) = 0;

lblImg = bwlabel(ws);

figure,imshow(label2rgb(lblImg,'jet','k','shuffle'));