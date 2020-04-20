function termtitle(termstring)
% TERMTITLE Sets title of the terminal.
%
%   termtitle;              % set the title to user@host
%   termtitle('blah');      % set the title to blah
%   termtitle(pwd);         % set the title to the value of pwd
%
%JC

if ispc,
%     fprintf('Can only set terminal title in LINUX, sorry.\n');
else
    if nargin<1 || isempty(termstring),
        [sys_status,sys_result]=system(['echo -n ${USER}@${HOST}']);
        if ~sys_status, termstring=sys_result;
        else termstring=pwd; end
    end
    if ~ischar(termstring), error('not a string'); end
    % [sys_status,sys_result]=system(['echo -n "\033]0;' termstring '\007"'])
    % if sys_status, warning('Setting terminal title did not work properly.'); end
    fprintf('\033]0;%s\007',termstring);
end
