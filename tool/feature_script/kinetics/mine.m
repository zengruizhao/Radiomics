clear;clc;close all;
addpath(genpath('../../feature_script'));
load ('../../data/preprocess/innertumoral/mm/C' );
C_label = peri_mask;
load ('../../data/preprocess/innertumoral/mm/P' );
P_label = peri_mask;
load ('../../data/preprocess/innertumoral/mm/V' );
V_label = peri_mask;
load ('../../label');
mm = 1;
%% extract kinetics features
x = [0 0.5 1];
for Case = 1:length(C_img)              
    if sum(sum(C_img{Case})) && sum(sum(P_img{Case})) && sum(sum(V_img{Case}))
        disp(Case);
        mean_haralick{Case, 1} = crop_mean_intensity(C_img{Case}, C_label{mm, Case});
        mean_haralick{Case, 2} = crop_mean_intensity(P_img{Case}, P_label{mm, Case});
        mean_haralick{Case, 3} = crop_mean_intensity(V_img{Case}, V_label{mm, Case});
        for i = 1:length(mean_haralick{2,1})
            y = [mean_haralick{Case, 1}(i), mean_haralick{Case, 2}(i), mean_haralick{Case, 3}(i)];
            fit_kinetics_haralick{1,Case}(1,(3*i-2):(3*i)) = polyfit(x',y',2);% ÄâºÏº¯Êı
        end
    end
end
save ('fit_kinetics_haralick', 'fit_kinetics_haralick');
%% plot determined curve
for Case = 1:length(C_img)              
    if sum(sum(C_img{Case})) && sum(sum(P_img{Case})) && sum(sum(V_img{Case}))
        disp(Case);
        for i = 1:length(label)
            if label(i,1) == Case
                G_label = label(i,2);
                if G_label == 1 
                    color = 'r'; 
                    linestyle = '-';
                    Legend = 'G1';
                else
                    color = 'b';
                    linestyle = '--';
                    Legend = 'G2';
                end
                ki67_label = label(i,3);
            end                   
        end
        mean_haralick{Case, 1} = crop_mean_intensity(C_img{Case}, C_label{Case});
        mean_haralick{Case, 2} = crop_mean_intensity(P_img{Case}, P_label{Case});
        mean_haralick{Case, 3} = crop_mean_intensity(V_img{Case}, V_label{Case});
        for i = 1 %which feature
%             y = [mean_haralick{Case, 1}(i), mean_haralick{Case, 2}(i) - mean_haralick{Case, 1}(i), mean_haralick{Case, 3}(i) - mean_haralick{Case, 1}(i)];
            y = [mean_haralick{Case, 1}(i), (mean_haralick{Case, 2}(i) - mean_haralick{Case, 1}(i)) / mean_haralick{Case, 1}(i) * 100, ...
                (mean_haralick{Case, 3}(i) - mean_haralick{Case, 1}(i)) / mean_haralick{Case, 1}(i) * 100];
            plot(x,y, 'Color', color, 'LineStyle', linestyle, 'LineWidth', 1.5);
            hold on;
            xlabel('Normalized time');
            ylabel('Diagonal derection gradient mean');
        end
    end
end
legend('G2', 'G1');
%% plot curve
% load kinetics_haralick;
% for j = 1:13
%     figure;
%     for Case = 1:length(C_img)
%         if sum(sum(C_img{Case})) && sum(sum(P_img{Case})) && sum(sum(V_img{Case}))
%             disp(Case);
%             for i = 1:length(label)
%                 if label(i,1) == Case
%                     G_label = label(i,2);
%                     if G_label == 1 color = 'r';else color = 'g';end
%                     ki67_label = label(i,3);
%                 end                   
%             end
%             y = [kinetics_haralick{Case, 1}(j), kinetics_haralick{Case, 2}(j), kinetics_haralick{Case, 3}(j)];
%             plot(x,y,color);
%             hold on;
%         end
%     end
%     hold off;
% end


