#include <iostream>
using namespace std;
struct ListNode {
	int val;
	ListNode * next;
	ListNode(int v):val(v),next(NULL) {}
};
class Solution {
public :
	void createList(ListNode *&head){
		int val=0;
		head = new ListNode(0);
		ListNode* node = head;
		while(cin>> val){
			ListNode* newNode = new ListNode(val);
			node->next = newNode;
			head->val++;
			node = node->next;
		}
	}
	ListNode* removeElement(ListNode* head,const int val){
		ListNode* node = head;
		while(node->next != NULL){
			ListNode* oldNode = node->next;
			if(oldNode->val==val){
				node->next=oldNode->next;
				delete oldNode;
				oldNode = NULL;
				head->val--;
			}
			else
			    node=node->next;
		}
		return head;
	}
	ListNode* removeElement_1(ListNode* head,const int val){
		while(head->next!=NULL && head->next->val==val){
			ListNode *node = head->next;
			head->next=node->next;
			delete node;
			node = NULL;
			head->val--;
		}
		ListNode* node = head->next;
		while(node->next != NULL){
			ListNode* oldNode = node->next;
			if(oldNode->val==val){
				node->next=oldNode->next;
				delete oldNode;
				oldNode = NULL;
				head->val--;
			}
			else
			    node=node->next;
		}
		return head;
	}


	void printList(const ListNode* head){
		cout << "length of list is " << head->val << endl;
		ListNode *node = head->next;
		while(node!=NULL){
			cout << node->val << " ";
			node=node->next;
		}
		cout << endl;
	}
};

int main(){
	Solution s;
	ListNode *head;
	s.createList(head);
	s.printList(head);
	s.removeElement_1(head,3);
	s.printList(head);
	return 0;
}



