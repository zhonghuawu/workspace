#include <iostream>

using namespace std;

struct ListNode {
	int val;
	ListNode *next;
	ListNode(int x):val(x),next(NULL) {}
};

class Solution {
public :
	void createList(ListNode *&head){
		int val=0;
		if(head == NULL){
			cin >> val;
			head = new ListNode(val);
		}
		ListNode* node = head;
		while(cin>> val && val != 0){
			ListNode* newNode = new ListNode(val);
			node->next = newNode;
			node = node->next;
		}
	}
	ListNode *addLists(ListNode *l1,ListNode *l2) {
		ListNode * node1 = l1;
		ListNode * node2 = l2;
		int addOne = 0;
		ListNode * result = NULL;
		ListNode * node_res = result;
		while(node1 != NULL && node2 != NULL){
			ListNode *node = new ListNode((node1->val+node2->val+addOne)%10);
			addOne = (node1->val + node2->val + addOne)/10;
			if(result == NULL){
				result = node;
				node_res = result;
			} else {
				node_res->next = node;
				node_res = node_res->next;
			}
			node1=node1->next;
			node2=node2->next;
		}
		if(node1 == NULL && node2 == NULL && addOne == 1){
			ListNode *node =  new ListNode(addOne);
			node_res->next = node;
		}
		if(node1 != NULL){
			ListNode *node = new ListNode(node1->val + addOne);
			node_res->next = node;
			node_res = node_res->next;
			node1=node1->next;
		}
		while(node1 != NULL){
			ListNode *node = new ListNode(node1->val);
			node_res->next = node;
			node_res = node_res->next;
			node1=node1->next;
		}
		if(node2 != NULL){
			ListNode *node = new ListNode(node2->val + addOne);
			node_res->next = node;
			node_res = node_res->next;
			node2=node2->next;
		}
		while(node2 != NULL){
			ListNode *node = new ListNode(node2->val);
			node_res->next = node;
			node_res = node_res->next;
			node2=node2->next;
		}
		return result;
	}
	void printList(ListNode* head){
		ListNode *node = head;
		while(node!=NULL){
			cout << node->val << " ";
			node=node->next;
		}
		cout << endl;
	}
};

int main(){

	ListNode *l1 = NULL;
	ListNode *l2 = NULL;
	Solution s;
	s.createList(l1);
	//cout << "l1: ";s.printList(l1);
	s.createList(l2);
	//cout << "l2: ";s.printList(l2);
	ListNode *result = s.addLists(l1,l2);
	s.printList(result);
	return 0;
}
