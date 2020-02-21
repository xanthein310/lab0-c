#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = (queue_t *) malloc(sizeof(queue_t));
    if (q == NULL)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    list_ele_t *temp;

    if (q == NULL)
        return;

    while (q->head != NULL) {
        temp = q->head->next;
        free(q->head->value);
        free(q->head);
        q->head = temp;
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;

    if (q == NULL)
        return false;

    newh = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;

    uint32_t string_size = strlen(s);
    newh->value = (char *) malloc(string_size + 1);
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    memcpy(newh->value, s, string_size);
    newh->value[string_size] = '\0';

    newh->next = q->head;
    q->head = newh;
    if (q->size == 0)
        q->tail = newh;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *newh;

    if (q == NULL)
        return false;

    newh = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;

    uint32_t string_size = strlen(s);
    newh->value = (char *) malloc(string_size + 1);
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    memcpy(newh->value, s, strlen(s));
    newh->value[string_size] = '\0';
    newh->next = NULL;

    if (q->tail != NULL)
        q->tail->next = newh;
    q->tail = newh;
    if (q->size == 0)
        q->head = newh;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    list_ele_t *temp;
    if (q == NULL)
        return false;
    if (q->size == 0)
        return false;

    if (sp) {
        uint32_t size_to_copy = strlen(q->head->value);
        if (size_to_copy > bufsize - 1)
            size_to_copy = bufsize - 1;
        memcpy(sp, q->head->value, size_to_copy);
        sp[size_to_copy] = '\0';
    }

    temp = q->head;
    q->head = q->head->next;
    q->size--;
    if (q->size == 0)
        q->tail = NULL;
    free(temp->value);
    free(temp);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q)
        return q->size;
    return 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    list_ele_t *current, *new, *temp;

    if (q == NULL)
        return;

    if (q->size == 0)
        return;

    current = q->head;
    new = NULL;
    for (int i = 0; i < q->size; i++) {
        temp = current->next;
        current->next = new;
        new = current;
        current = temp;
    }
    q->tail = q->head;
    q->head = new;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    list_ele_t *current, *prev, *temp;

    if (q == NULL)
        return;

    if (q->size == 0)
        return;

    for (int i = q->size; i > 0; i--) {
        current = q->head;
        prev = q->head;
        for (int j = 0; j < i - 1 && current->next; j++) {
            if (strcasecmp(current->value, current->next->value) > 0) {
                temp = current->next;
                current->next = temp->next;
                temp->next = current;

                if (current == q->head) {
                    q->head = temp;
                    prev = temp;
                } else {
                    prev->next = temp;
                    prev = prev->next;
                }
            } else {
                current = current->next;
                if (j != 0)
                    prev = prev->next;
            }
        }
    }

    q->tail = q->head;
    while (q->tail->next)
        q->tail = q->tail->next;
}
