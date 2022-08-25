make re
OUTPUT=$(ls config_files_tests)
#valgrind --track-fds=yes ./webserv $OUTPUT
./webserv $OUTPUT
