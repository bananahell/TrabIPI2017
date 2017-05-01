I=imread('car.png');
imshow(I)
G=imadjust(I,[0 1],[1 0]); % o negativo também pode ser feito com g = imcomplement(f)
figure;imshow(G)
G=imadjust(I,[0.5 0.75],[0 1]);
imshow(G)
I=imadjust(I,[ ],[ ],2);
imshow(G);