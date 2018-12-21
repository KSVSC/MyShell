# MyShell
Implementing shell in C

## Implemented Commands:
<br> pwd
<br> cd (cd ..,cd~,cd <filename>)
<br> echo (takes care of double quotes)
<br> ls [al] (ls -a,ls -l,ls -al,ls -la)
<br> pinfo
<br> setenv var [value]
<br> unsetenv var
  jobs
  kjob <job_num> <sig_num> (kill job)
  fg <job_num> (brings bg process to fg process)
  bg <job_num> (puts stopped bg process to running state)
  overkill
  quit
  Ctrl Z
  Ctrl C
  

<br> Taken care of forground and Background(&) processes.
<br> If the background process exits then the shell must display the appropriate
message to the user.
<br> Implemented Input-Output Redirection(>,<,>>).
<br> Redirection using pipes
<br> Implement i/o redirection + pipes redireciton

### Extra Commands:
<br> remindme <time> <Remainder>
<br> clock -t <timer>


