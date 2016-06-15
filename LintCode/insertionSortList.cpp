#include <iostream>

using namespace std;
struct ListNode {
	int val;
	ListNode* next;
	ListNode(int x):val(x), next(NULL) {}
};

class Solution {
public:
    /**
     * @param head: The first node of linked list.
     * @return: The head of linked list.
     */
    ListNode *insertionSortList(ListNode *head) {
        // write your code here
        typedef ListNode* Node;
        Node pnode = head->next;
        head->next = NULL;
        while(pnode!=NULL){
            Node p = head;
            Node p_pre = head;
            while(p!=NULL && p->val < pnode->val)
                p_pre = p, p=p->next;
            if(p == head){
                head = pnode;
                pnode = pnode->next;
                head->next = p;
            }else {
                p_pre->next = pnode;
                pnode = pnode->next;
                p_pre->next->next = p;
            }
        }
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
	int a[] = {3,2,4};
	createList(head,a,sizeof(a)/sizeof(int));
	printList(head);
	Solution s;
	head = s.insertionSortList(head);
	printList(head);
}
