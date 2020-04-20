%% Funtion to create a mask from xml annotation 
%


function maskSl = getMaskFromXml(annot, xSize, ySize, distTh)
    maskSl = zeros (xSize, ySize);
    for j = 1:size(annot,2)

        indPoint    =  1;
        firstPoint1  = [annot(j).X(indPoint) annot(j).Y(indPoint)];
        indPoint    = size(annot(j).X,1);
        lastPoint1   = [annot(j).X(indPoint) annot(j).Y(indPoint)];
        distP        = sqrt(sum((firstPoint1 - lastPoint1).^2));
	
        status = '';
        if (distP < distTh)
            status              = ' closed region -                  ';
            tempMask    		= poly2mask(annot(j).X,annot(j).Y,xSize, ySize);
            maskSl(tempMask) 	= 1;
            annot(j).used 		= 1;
        else
            status              = '  open region  - will collate next';
            annot(j).used 		= 0;
        end
        str_j           = sprintf('% 3d',j);
        disp([str_j ' ' num2str(annot(j).type) ' Note:(' annot(j).text{1} ') ' num2str(annot(j).used)  status  ' (dist = ' num2str(distP) ')' ]);
    end
    
    isOpen = false;

    for j = 1:size(annot,2)
        if (annot(j).used == 0)
            disp([num2str(j) ]);
            closedAnnot.X = [];
            closedAnnot.Y = [];
            closedAnnot.X = [closedAnnot.X; annot(j).X];
            closedAnnot.Y = [closedAnnot.Y; annot(j).Y];
            
            isOpen = true;
            indPoint    =  1;
            firstPoint  = [annot(j).X(indPoint) annot(j).Y(indPoint)];
            indPoint    = size(annot(j).X, 1);
            lastPoint   = [annot(j).X(indPoint) annot(j).Y(indPoint)];

            annot(j).used = 1;

            % finding neighbor to the last
            i = 1; 
            lastPointAnnotId = j;
            while( i <= size(annot,2))
                 %[ j i lastPointAnnotId i ~= lastPointAnnotId]
                if (i ~= lastPointAnnotId);
                    indPoint    =  1;
                    firstPoint2 = [annot(i).X(indPoint) annot(i).Y(indPoint)];

                    indPoint    = size(annot(i).X, 1);
                    lastPoint2  = [annot(i).X(indPoint) annot(i).Y(indPoint)];

                    distL2F     = sqrt(sum((firstPoint2 - lastPoint).^2)); % distance last to first
                    distL2L     = sqrt(sum((lastPoint2  - lastPoint).^2)); % distance last to last
    
                    str_i       = sprintf('% 3d',i);
                    str_l       = sprintf('% 3d',lastPointAnnotId);

                    if (distL2F<distTh)
                        %just concatenate
                        neighborstatus = [ ' connect ' str_l ' to         ' str_i ' (dist = ' num2str(distL2F) ')' ]; 
                        disp(neighborstatus);    
                        if (annot(i).used == 1);
                            i = size(annot,2)+1; %exit found the end
                            tempMask    		= poly2mask(closedAnnot.X, closedAnnot.Y ,xSize, ySize);
                            maskSl(tempMask) 	= 1;
                        else
                            closedAnnot.X = [closedAnnot.X; annot(i).X];
                            closedAnnot.Y = [closedAnnot.Y; annot(i).Y];
                            indPoint    = size(closedAnnot.X, 1);
                            lastPoint   = [closedAnnot.X(indPoint) closedAnnot.Y(indPoint)];
                            lastPointAnnotId = i;
                            annot(i).used = 1;
                            i = 1;  %not found the end
                        end

                        

                    else
                        if (distL2L < distTh)
                            %flip and concatenate
                            neighborstatus = [ ' connect '  str_l  ' to flipped ' str_i ' (dist = ' num2str(distL2L) ')' ]; 
                            disp(neighborstatus); 
                            if (annot(i).used == 1);
                                i = size(annot,2)+1; %exit found the end
                                tempMask    		= poly2mask(closedAnnot.X, closedAnnot.Y , xSize, ySize);
                                maskSl(tempMask) 	= 1;
                                isOpen = false;
                             else
                                inds = size(annot(i).X,1) - (0:(size(annot(i).X,1)-1));
                                closedAnnot.X = [closedAnnot.X; annot(i).X(inds)];
                                closedAnnot.Y = [closedAnnot.Y; annot(i).Y(inds)];
                                indPoint    = size(closedAnnot.X, 1);
                                lastPoint   = [closedAnnot.X(indPoint) closedAnnot.Y(indPoint)];
                                lastPointAnnotId = i;
                                annot(i).used = 1;
                                i = 1;  %not found the end
                             end
                             
                        else
                            %keep seachrcing
                            neighborstatus = [ ' ' num2str(i) ' not found (' num2str(distL2F) '-' num2str(distL2L) ')'  ]; 
                            i = i + 1;
                        end
                     end
               %disp(neighborstatus);

             else
                i = i + 1;
             end
            end
        
        end    
    end

    if (isOpen);
       tempMask             = poly2mask(closedAnnot.X, closedAnnot.Y ,xSize, ySize);
       maskSl(tempMask) 	= 1;
    end
   

end
