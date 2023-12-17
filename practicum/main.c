#include "memory_simulation.h"
#include "memory_cache.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


linkedlist test_initialized_linkedlist() {
	linkedlist list;
	list.head = NULL;
	list.tail = NULL;
	list.count = 0;
	return list;
}

void free_linkedlist(linkedlist list) {
	node* current = list.head;
	while (current != NULL) {
		node* next = current->next;
		free(current);
		current = next;
	}
}

void test_normal_cases() {
	/** 
	Test regular message creation then store it in both disk and cache.
	Testing strategy:
	- Adding exactly 16 message (cache only has 16 slots)
	- 16 files are written to disk 
	- 16 message are now written to a doubly linked list (this is how I implement cache)
	- To ensure file is written correctly to disk:
		- Check the folder to see if there are indeed 16 txt file
		- Check message 13, 14, 15, and 16 to see if the sender, receiver and content 
		are within its boundary of 50, 50, 200 length respectively
	- To ensure 16 message are written to cache:
		- print the id of the doubly linked list (id is actually the unique id of the message)
		- retrieve message 5 then print the id (to make sure that we implement LRU correctly)
			- Expect to see unique_id 5 on top follow by the rest of the id
		- retrieve message 1 then print the id
			- Expect to see unique_id 1 followed by 5 then the rest
		- retrieve message 10 then print the id
			- Expect to see unique_id 10 followed by 1 then 5 then the rest
	
	With all these, we are able to confirm that the load node to cache works correctly and cache hit or search
	is also working correctly as well.
	Now we are going to test if removing cache out of the node works as expected with two algorithm randomnode and LRU.
	- Add message 17 into the cache node using LRU. Then print the nodes out, node 2 will be missing now
	- Add message 18 into the cache node using randomnode. Then print the nodes out, random node will be missing and
	node 18 will become the most recent used node.
	*/
	message message1 = create_msg(1, "leaksmy_heng", "mey_mey", "hello this is a test only");
	message message2 = create_msg(2, "leaksmy_heng@gmail.com","me@gmail.com", "I hope you have a good day Best,Leaksmy");
	message message3 = create_msg(3, "hello_@gmail.com", "you@gmail.com", "Guinea pig is the cutest animal Best,Leaksmy");
	message message4 = create_msg(4, "meh@gmail.com", "may@gmail.com", "Hope everyone's dream come true Best,Leaksmy");
	message message5 = create_msg(5, "youtube@gmail.com", "june@gmail.com", "Please have a good day Best,Leaksmy");
	message message6 = create_msg(6, "monday@gmail.com", "junly@gmail.com", "World peace Best,Leaksmy");
	message message7 = create_msg(7, "tuesday@gmail.com", "august@gmail.com", "Get promoted Best,Leaksmy");
	message message8 = create_msg(8, "wednesday@gmail.com", "september@gmail.com", "Graduate soon Best,Leaksmy");
	message message9 = create_msg(9, "thursday@gmail.com", "october@gmail.com", "Korean drama is funny Best,Leaksmy");
	message message10 = create_msg(10, "friday@gmail.com", "november@gmail.com", "Thank you for all you have done Best,Leaksmy");
	message message11 = create_msg(11, "saturday@gmail.com", "december@gmail.com", "Everyone deserves the best.");
	message message12 = create_msg(12, "march@gmail.com", "me@gmail.com", "hello this is a test only Best,Leaksmy");
	// this is the message in which the sender or reciver or content is longer than the specify character.
	message message13 = create_msg(13, "april@gmail.com,me@gmail.com,friday@gmail.com,tuesday@gmail.com,meh@gmail.com,leaksmy_heng@gmail.com,meh@gmail.com", "april@gmail.com,me@gmail.com,friday@gmail.com,tuesday@gmail.com,meh@gmail.com,leaksmy_heng@gmail.com,meh@gmail.com", "april@gmail.com,me@gmail.comhello this is a test only Best,Leaksmy");
	message message14 = create_msg(14, "sunday@gmail.com,january@gmail.com,february@gmail.com,march@gmail.com,april@gmail.com,may@gmail.com", "me@gmail.com", "This is the test when sender's length is more than 50.");
	message message15 = create_msg(15, "sunday@gmail.com,january@gmail.com,february@gmail.com,march@gmail.com,april@gmail.com,may@gmail.com", "me@gmail.com", "hello this is a test only Best,Leaksmy. This is the test when receiver's length is more than 50.");
	message message16 = create_msg(16, "test_content_out_of_length", 	"me@gmail.com", "Hello this is leaksmy heng talking. I am from Cambodia. I have been in school for a very very long time. I hope to graduate soon then I could just work without worrying about the deadline. But I do enjoy every class in northeastern so far. This is a nice school and a lot of professor are really attentive to the student. The TA programs help a ton too because when I am stuck in the homework, I could just come to the TA for help. This is actually just a test for when the content is longer than 200. I appriciate you grading my work! Have a good day ^^.");
	
	// initialized doubly linked list
	linkedlist list = test_initialized_linkedlist();
	
	// Test store messsage at random
	store_msg(&list, message1, true);
	store_msg(&list, message2, true);
	store_msg(&list, message3, true);
	store_msg(&list, message4, true);
	store_msg(&list, message5, true);
	store_msg(&list, message6, true);
	store_msg(&list, message7, true);
	store_msg(&list, message8, true);
	store_msg(&list, message9, true);
	store_msg(&list, message10, true);
	store_msg(&list, message11, true);
	store_msg(&list, message12, true);
	store_msg(&list, message13, true);
	store_msg(&list, message14, true);
	store_msg(&list, message15, true);
	store_msg(&list, message16, true);
	
	printlinkedlist(&list);
	message rmessage5 = searchcache(&list, 5);
	printlinkedlist(&list);
	message rmessage1 = searchcache(&list, 1);
	printlinkedlist(&list);
	message rmessage10 = searchcache(&list, 10);
	printlinkedlist(&list);
	
	// insert new message into the cach, this would make the cache delete one node.
	// The node to delete would be node 2 (LRU implementation). Print the linkedlist out, and verify
	// that node 2 is gone
	message message17 = create_msg(17, "test_removenode2", 	"test_removenode2", "test_removenode2");
	store_msg(&list, message17, false);
	printlinkedlist(&list);
	
	// insert new node into cache, this time implement randomnode.
	// printlinkedlist at the end will see the new node, but one random node will be gone
	message message18 = create_msg(18, "test_removenoderandom", 	"test_removenoderandom", "test_removenoderandom");
	store_msg(&list, message18, true);
	printlinkedlist(&list);
	
	// free the doubly linked list
	free_linkedlist(list);
}


void test_edge_cases_randomnode() {
	/**
	Inserting 25 messages in total, this means some messages will have to be removed so that other messages can be inserted into 
	cache. To verify that it works correctly:
	- load message 20 to 45 into the folder and cache data structure (linkedlist)
	- when hitting 36 id, the message stating "Linkedlist is too full. Remove one node now" will pop up
	(uncomment line 134 in memory_cache.c for to see that)
	- printlinkedlist to see that now there are id in there but not in order because we pop some out at random
	
	To test that the retrieve_msg is working correctly (retrieve from both cache and disk):
		- Loop through index 20 to 45 because we inserted message with those id
		- if the message id is in cache, we will not see the message indicating that
			"Message was not able to find in cache. Therefore, checking in disk now"
		- if the message is not in cache, we will see the message above
		- print the content of the message (like unique_id, timesent, sender, reciever, and so on)
	*/
	
	// initialized doubly linked list
	linkedlist list = test_initialized_linkedlist();
	
	// adding messages into cache
	for (int i=20; i<45; i++) {
		message message = create_msg(i, "leaksmy_heng", "mey_mey", "hello this is a test only");
		store_msg(&list, message, true);
	}
	
	// print the linkedlist out to verify that the insert works correctly
	printlinkedlist(&list);
	
	// retreive the message
	for (int i=20; i<45; i++) {
		message rmessage = retrieve_msg(&list, i);
		print_msg(rmessage);
	}
	
	// free the doubly linked list
	free_linkedlist(list);
}

void test_edge_cases_lru() {
	/**
	Testing least recently added algorithm:
	- initialized linked list
	- populate 16 messages into the cache aka linked list
	- print the linked list to see that the id is indeed from 50 to 66
	- retreive message 50, 55, and 60. Then print the linked list. We expect to see 60, 55, 50 as the top 3 of the least
	and 51 is in the bottom the least.
	- Now add new message id in. Add message id 70, 71, 72. Therefore, the message 51, 52, 53 that was on the list will be remove.
	- print the linked list. Now we should see 72, 71, 70, 60, 55, 50 on top of the least
	*/
	
	// initialized doubly linked list
	linkedlist list = test_initialized_linkedlist();
	
	//populate 16 messages into the cache aka linked list and this is lru 
	// therefore, in the store_msg there is false
	for (int i=50; i<=66; i++) {
		message message = create_msg(i, "test_lru", "test_lru", "hello this is a test_lru only");
		store_msg(&list, message, false);
	}
	
	// print the linked list to see that the id is indeed from 50 to 66
	printlinkedlist(&list);
	
	// retreive message 50, 55, and 60.
	message rmessage50 = retrieve_msg(&list, 50);
	message rmessage55 = retrieve_msg(&list, 55);
	message rmessage60 = retrieve_msg(&list, 60);
	
	// print the linked list to see that the id is indeed from 50 to 66
	printlinkedlist(&list);
	
	// Add message id 70, 71, 72.
	message message70 = create_msg(70, "test_lru", "test_lru", "hello this is a test_lru only");
	message message71 = create_msg(71, "test_lru", "test_lru", "hello this is a test_lru only");
	message message72 = create_msg(72, "test_lru", "test_lru", "hello this is a test_lru only");
	store_msg(&list, message70, false);
	store_msg(&list, message71, false);
	store_msg(&list, message72, false);
	
	// print the linked list. Now we should see 72, 71, 70, 60, 55, 50 on top of the least
	printlinkedlist(&list);
	
	// free the doubly linked list
	node* current = list.head;
	while (current != NULL) {
		node* next = current->next;
		free(current);
		current = next;
	}
}


void random_hits_misses(linkedlist* list, int* hits, int* misses, bool is_random_node) {
	/**
	Create a function to calculate the number of hits or misses with 1000 random messages.
	In this case I have made an assumption that the random id found in those 1000 message are between
	1 and 50.
	Logic:
	- loop through the message between 1 and 1000
	- assume that the random number is between 1 and 50
	- if the random number is in the prepopulated cache, increment the hit
	- else increment the misses and insert that new random number into the cache as its unique id
	
	Note: 
	- is_random_node is a flag for the algorithm we are using.
	*/
	
	// get random number
	srand((unsigned int)time(NULL));
	
	// loop through message 0 to 1000
	for (int i=0; i<1000; ++i) {
		// getting the message with id 1-50
		// this is the assumption that I made that the message id is between 1 and 50
		int randomid = rand() % 50 + 1;
		
		// check if message is in cache, if it is not in cache, increment the miss and insert it into node
		message data = searchcache(list, randomid);
		if ((data.unique_identifier==0) && (data.timesent==0) && (data.flag==0)) {
			(*misses)++;
			message data = create_msg(randomid, "random_message_sender", "random_message_receiver", "random_message_content");
			insertnode(list, data, is_random_node);
		}
		else {
			(*hits)++;
		}
	}
}

void test_random_message_hits_misses() {
	/**
	Function to calculate the cache metrics.
	
	Metrics: 
	- hitsper1000accesses = Number of hits / 1000
	- missesper1000accesses = Number of misses / 1000
	- cachehitratioper1000accesses = Number of hits / (Number of hits + Number of misses)
	
	In hit ratio per 1000 accesses, I multiply it with 1000 because it is with 1000 accesses
	
	*/
	
	// initialized doubly linked list for lru and random_node
	linkedlist list_lru = test_initialized_linkedlist();
	linkedlist list_random_nod = test_initialized_linkedlist();
	
	// adding 16 message to caches (both lru and random_node)
	for (int i=1; i<=16; i++) {
		message data = create_msg(i, "test_random_message_hits_misses", "test_random_message_hits_misses", "hello this is a tetest_random_message_hits_missesst_lru only");
		insertnode(&list_lru, data, false);
		insertnode(&list_random_nod, data, true);
	}
	
	// print the linked list to verify that the node is populated correctly in lru and random_node
	printf("\nPrint linked list for LRU\n");
	printlinkedlist(&list_lru);
	printf("\nPrint linked list for Random Node\n");
	printlinkedlist(&list_random_nod);
	
	printf("\n--- Metrics for Least Recently Used ---\n");
	int hits_lru = 0;
	int misses_lru = 0;
	random_hits_misses(&list_lru, &hits_lru, &misses_lru, false);
	double hitsper1000accesses_lru = (double)hits_lru/1000.0;
	double missesper1000accesses_lru = (double)misses_lru/1000.0;
	double hitratio_lru = (double)hits_lru/(hits_lru + misses_lru);
	printf("%d misses lru\n", misses_lru);
	printf("%d hits lru\n", hits_lru);
	printf("Hits per 1000 access metrics: %.2f\n", hitsper1000accesses_lru);
	printf("Misses per 1000 access metrics: %.2f\n", missesper1000accesses_lru);
	printf("Hit ratio: %.2f\n", hitratio_lru);
	
	printf("\n--- Metrics for Random Node ---\n");
	int hits_random_nod = 0;
	int misses_random_nod = 0;
	random_hits_misses(&list_random_nod, &hits_random_nod, &misses_random_nod, true);
	double hitsper1000accesses_random_nod = (double)hits_random_nod/1000.0;
	double missesper1000accesses_random_nod = (double)misses_random_nod/1000.0;
	double hitratio_random_nod = (double)hits_random_nod/(hits_random_nod + misses_random_nod);
	printf("%d misses random node\n", misses_random_nod);
	printf("%d hits random node\n", hits_random_nod);
	printf("Hits per 1000 access metrics: %.2f\n", hitsper1000accesses_random_nod);
	printf("Misses per 1000 access metrics: %.2f\n", missesper1000accesses_random_nod);
	printf("Hit ratio: %.2f\n", hitratio_random_nod);
	
	// free lru and random node
	free_linkedlist(list_lru);
	free_linkedlist(list_random_nod);
}


void main() {
	printf("---------------Testing test_normal_cases --------------\n");
	test_normal_cases();
	printf("---------------Testing test_edge_cases_randomnode --------------\n");
	test_edge_cases_randomnode();
	printf("---------------Testing test_edge_cases_randomnode --------------\n");
	test_edge_cases_lru();
	printf("---------------Testing test_random_message_hits_misses --------------\n");
	test_random_message_hits_misses();
}
