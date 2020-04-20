% 7/18/2012 40 patients, 41 images sample links for

link.casenumber = ...
    [ {'04-0709'} {'04-0518'} {'04-0521'} {'04-0606'}  {'04-0715'} {'06-1303'} {'02-0006'}  {'03-0089'} {'03-0061'} {'03-0001'} {'02-0004'}...
    {'02-0047'} {'02-0021'} {'02-0300'} {'02-0022'} {'04-0509'} {'04-0527'} {'04-0527'} {'04-0624'} {'04-0746'} {'05-1006'}...
    {'1B4'} {'2A3'} {'3B1'} {'6E4'} {'7C3'} {'8C1'} {'9C2'} {'11A3'} {'13B1'} {'14C6'} {'15B4'} {'16C6'}...
    {'18C3'} {'19D1'} {'20A3'} {'21A2B'} {'22B4'} {'23B2'} {'24D2'} {'26C4'}]

link.aperioID = ...
    [ {'20704'} {'20706'} {'20710'} {'20712'} {'20714'} {'20693'} {'20695'} {'20698'} {'20700'} {'20702'} {'21630'} {'21632'} {'21753'} {'21757'} {'21759'} {'22213'} {'22217'} {'22219'} {'22221'} {'22225'} {'22226'} {'22973'} {'22974'} {'22976'} {'22977'} {'22980'} {'22861'} {'22862'} {'22858'} {'22981'} {'22863'} {'22864'} {'22873'} {'22982'} {'22876'} {'22878'} {'22983'} {'22984'} {'22985'} {'22881'} {'22882'}]

link.status = link.status'


biopsy_index = [22977       22980       22861       22981       22863       22873       22982  ...
    22876       22878       22983       22984       22985      22881       22882]

multiple_regions = []

%%
mult = {}
for i = 1:length(dataset)
    regionfolder = {};
    count = [];
    if ismember(str2num(dataset{i}),biopsy_index)
        for r = 1:50
            if exist([results_dir filesep dataset{i} filesep 'region' num2str(r)],'dir')
                regionfolder{r} = [dataset{i} 'region' num2str(r)];
            end
        end
        %fprintf(['\n Dataset ' num2str(dataset{i}) ' has ' num2str(length(regionfolder)) ' regions']);
        if length(regionfolder) == 1
            mult(end+1:end+length(regionfolder)) = regionfolder
        end
    end
end


disambiguation = [{'22878region1'} {'22882region2'} {'22882region3'} {'22983region2'} {'22984region2'}]


mult(end+1:end+length(disambiguation)) = disambiguation
biopsy_regions = mult

save('56points-biopsy_regions.mat', 'biopsy_regions')
