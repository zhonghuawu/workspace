#include <iostream>

using namespace std;
struct ListNode {
	int val;
	ListNode* next;
	ListNode(int x):val(x), next(NULL) {}
};

void createList(ListNode*&head,int a[],int sz);
void printList(ListNode*head);
class Solution {
public:
	ListNode* deleteDuplicates(ListNode *head){
		typedef ListNode* Node;
		if(head == NULL)
		  return head;
		Node root = new ListNode(0);
		root->next = head;
		Node pre = root;
		Node pnode, pend;
		while(pend!=NULL){
			pnode = pre->next;
			pend = pnode->next;
			while(pend!=NULL && pnode->val == pend->val)pend = pend->next;
			if(pnode->next == pend){
				pre = pre->next;
			}else{
				pre->next = pend;
				while(pnode!=pend){
					Node tmp = pnode;
					pnode = pnode->next;
					delete tmp;
				}
			}
		}
		head = root->next;
		delete root;
		root = NULL;
		return head;
	}		
};
void createList(ListNode*&head, int a[], int sz){
	head = new ListNode(a[0]);
	ListNode* p = head;
	for(int i=1;i<sz;i++){
		ListNode* tmp = new ListNode(a[i]);
		p->next = tmp;
		p=p->next;
	}
}
void printList(ListNode*head){
	while(head!=NULL){
		cout << head->val << " ";
		head = head->next;
	}
	cout << endl;
}

int main(){
	ListNode* head;
	int a[] = {0,1,1,2,3,4,4,5,6,6};
	createList(head,a,sizeof(a)/sizeof(int));
	printList(head);
	Solution s;
	head = s.deleteDuplicates(head);
	printList(head);
}
