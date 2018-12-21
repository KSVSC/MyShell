# MyShell
Implementing shell in C

## Implemented Commands:
<br> 1.pwd
<br> 2.cd (cd ..,cd~,cd <filename>)
<br> 3.echo (takes care of double quotes)
<br> 4.ls [al] (ls -a,ls -l,ls -al,ls -la)
<br> 5.pinfo
<br> 6.setenv var [value]
<br> 7.unsetenv var
<br> 8.jobs
<br> 9.kjob <job_num> <sig_num> (kill job)
<br> 10.fg <job_num> (brings bg process to fg process)
<br> 11.bg <job_num> (puts stopped bg process to running state)
<br> 12.overkill
<br> 13.quit
<br> 14.Ctrl Z
<br> 15.Ctrl C

###  Implementation:
<br> Taken care of forground and Background(&) processes.
<br> If the background process exits then the shell must display the appropriate
message to the user.
<br> Implemented Input-Output Redirection(>,<,>>).
<br> Redirection using pipes
<br> Implement i/o redirection + pipes redireciton

### Extra Commands:
<br> remindme <time> <Remainder>
<br> clock -t <timer>


