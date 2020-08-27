# project1

Driverless skip list implementation from the skip list cookbook provided in project 1 document.

Skip list is used to store messages with access-control list.

mbx_init - This system call is implemented using an overhead function in skip_list.c. The function returns a skip list pointer to the newly initialized skip list. The maximum number of levels and probability to add an additional level are passed in form the syscall and the member variables are set for the struct. Head contains an id with the maximum value for long. This is used as a sentinel. The forward pointer, as well as the random number are initialized. Level is set to one and size to 0. Returns a pointer to the skip list.

mbx_shutdown - This system call is implemented using an overhead function in skip_list.c. The function calls another  helper function to destroy individual nodes. This helper function calls another helper function to destroy individual messages. Returns 0 or 1 based on success.

mbx_create - This system call is implemented using an overhead function in skip_list.c The function calls the insert function in my skip_list file. This function creates and inserts a node according to the id, then raises the node’s level based on a randomly generated int and probability.

mbx_destroy - This system call is implemented using an overhead function in skip_list.c. This function finds the node based on the given id. It then deletes all messages within the mailbox, and then frees the memory, respectfully.

mbx_count - This system call is implemented using an overhead function in skip_list.c. This function returns the tracked size of the mailbox using a member variable in the mailbox struct.

mbx_send - This system call is implemented with some help from an overhead function in skip_list.c This function takes in the shallow copy message and adds it to the mailbox based on the given id.

mbx_recv - I was unable to correctly implement this function. Most of the things I tried gave me huge errors so I just have it as return 0

mbx_length - This function uses sizeof to return the number of bytes based on the length of the message.

