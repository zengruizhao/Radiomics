function [RF,header] = ReadEye(filename)

fid      = fopen(filename,'r');
status   = fseek(fid, 16,'bof'); rfpts  = fread(fid,1,'int16');
status   = fseek(fid, 18,'bof'); lines  = fread(fid,1,'int16');
status   = fseek(fid, 20,'bof'); fs     = fread(fid,1,'int16');
status   = fseek(fid, 22,'bof'); btsize = fread(fid,1,'uint16');            % A/D bits size
status   = fseek(fid, 24,'bof'); btword = fread(fid,1,'uint16');            % bytes per A/D word
status   = fseek(fid, 48,'bof'); del    = fread(fid,1,'float32');
status   = fseek(fid,176,'bof'); vscale = fread(fid,1,'single');
status   = fseek(fid,180,'bof'); gain   = fread(fid,1,'single');
status   = fseek(fid,184,'bof'); att    = fread(fid,1,'single');
status   = fseek(fid,266,'bof'); c      = fread(fid,1,'int16');
status   = fseek(fid,286,'bof'); energy = fread(fid,1,'uint8');
status   = fseek(fid,287,'bof'); dampg  = fread(fid,1,'uint8');
status   = fseek(fid,288,'bof'); hpfilt = fread(fid,1,'uint8');
status   = fseek(fid,289,'bof'); lpfilt = fread(fid,1,'uint8');
status   = fseek(fid,295,'bof'); DeltaX = fread(fid,1,'int16');
status   = fseek(fid,297,'bof'); DeltaY = fread(fid,1,'int16');
header   = struct(  'rfpts',        rfpts,...
                    'lines',        lines,...
                    'fs',           fs*1e6,...
                    'bitsize',      btsize,...
                    'del',          del*1e-3,...
                    'voltscale',    vscale,...
                    'gain',         gain,...
                    'attenuation',  att,...
                    'c',            c,...
                    'energy',       energy,...
                    'damping',      dampg,...
                    'HPfilter',     hpfilt,...
                    'LPfilter',     lpfilt,...
                    'DeltaX',       DeltaX*1e-6,...
                    'DeltaY',       DeltaY*1e-6);
% skip header and read rf data
status   = fseek(fid, 12,'bof'); hd     = fread(fid,1,'int16');
status   = fseek(fid, hd,'bof'); data   = fread(fid,inf,['bit' num2str(btsize)],8*btword-btsize);

% reshape vector data to matrix of rf lines
RF = reshape(data, rfpts, lines);