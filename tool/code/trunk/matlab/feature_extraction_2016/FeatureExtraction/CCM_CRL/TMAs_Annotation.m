
% in the 'WUSTL_Oral_Histology' data we have 6 TMAs, where each one has a
% few cores we interested in.
% we have in total 117 cases/cores, the annoation please checke the tif
% files.

setRange{1}=1:18; % the annotated range for TMA 
setTMALayout{1}=[0 0 0 0 0 1 0 0 2 0 0 0;
                 0 0 0 5 0 0 4 0 0 0 0 3;
                 6 0 0 0 7 0 0 0 0 8 0 0;
                 0 0 0 11 0 0 10 0 0 0 0 9;
                 12 0 0 0 0 0 0 0 13 0 0 0;
                 0 0 15 0 0 0 0 14 0 0 0 0;
                 0 16 0 0 0 17 0 0 0 18 0 0];
setRange{2}=19:38;  
setTMALayout{2}=[0 0 0 0 19 0 0 0 0 20 0 0;
                 0 0 0 23 0 0 22 0 0 0 21 0;
                 0 24 0 0 0 25 0 0 0 26 0 0;
                 0 0 29 0 0 0 28 0 0 0 27 0;
                 30 0 0 0 0 31 0 0 0 32 0 0;
                 0 0 35 0 0 0 34 0 0 0 33 0;
                 36 0 0 0 0 37 0 0 38 0 0 0];
setRange{3}=[39:51 53:57];   % note that there is a number missing
setTMALayout{3}=[0 0 0 0 39 0 0 0 0 40 0 0;
                 0 0 0 0 0 0 0 42 0 0 41 0;
                 43 0 0 0 0 44 0 0 0 45 0 0;
                 0 0 0 48 0 0 47 0 0 0 46 0;
                 49 0 0 0 0 50 0 0 0 51 0 0;
                 0 0 0 54 0 0 0 53 0 0 0 0;
                 55 0 0 0 56 0 0 0 0 57 0 0];
setRange{4}=58:68;  
setTMALayout{4}=[0 0 0 0 0 58 0 0 0 59 0 0;
                 0 0 62 0 0 0 0 61 0 0 60 0;
                 0 63 0 0 64 0 0 0 0 65 0 0;
                 0 0 68 0 0 0 0 67 0 0 66 0];
setRange{5}=69:104;  
setTMALayout{5}=[0 0 0 0 0 69 70 0 71 0 72 0;
                 0 77 76 0 0 0 0 75 74 0 73 0;
                 0 78 79 0 80 0 0 81 82 0 83 0;
                 88 0 0 0 87 0 0 86 0 85 0 84;
                 0 89 90 0 91 0 0 0 92 0 93 0;
                 99 0 98 0 97 0 96 0 0 95 94 0;
                 0 0 100 0 0 101 102 0 103 0 0 104];
setRange{6}=105:118;  
setTMALayout{6}=[0 0 105 0 0 0 106 0 107 0 108 0;
                 0 112 111 0 0 0 0 110 0 0 0 109;
                 113 0 0 114 115 0 0 116 117 0 0 118];
