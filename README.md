# Description

## Part A : Basic System Calls
### Specification 1 : Display Requirement
On every line when waiting for user input, a shell prompt of the following form must appear along with it.

```<Username@SystemName:~>```


For example, if system name is “SYS”, username is “JohnDoe”, and the user is currently in the home directory, then prompt looks like this :

```<JohnDoe@SYS:~>```


The directory from which shell is invoked becomes the home directory for the shell and represented with “~”. All paths inside this directory should be shown relative to it. Absolute path of a directory/file must be shown when outside the home directory.

When user changes the working directory, the corresponding change in path of directory must be reflected in the next prompt. For example, on going to the parent directory of the home directory of shell, following form of prompt is expected :


```<JohnDoe@SYS:/home/johndoe/sem3>```

### Specification 2 : Input Requirements [5]
Keep in mind the following requirements for input when implementing your shell :

Your shell should support a ‘;’ or ‘&’ separated list of commands. You can use ‘strtok’ to tokenize the input.
Your shell should account for random spaces and tabs when taking input.
The “;” command can be used to give multiple commands at the same time. This works similar to how “;” works in Bash.
‘&’ operator runs the command preceding it in the background after printing the process id of the newly created process.


```./a.out```

```<JohnDoe@SYS:~> vim      &```
[1] 35006

```bash
<JohnDoe@SYS:~> sleep 5 &     echo     "Lorem ipsum"
[2] 35036
Lorem ipsum

```

##### sleep runs in the background while echo runs in the foreground

```bash
<JohnDoe@SYS:~> warp test ; pwd
sleep with pid 35036 exited normally # after 5 seconds
~/test
```

```<JohnDoe@SYS:~/test> ```
If any command is erroneous, then error should be printed.

```bash
<JohnDoe@SYS:~> sleeeep 6
ERROR : 'sleeeep' is not a valid command
```
# Assumptions