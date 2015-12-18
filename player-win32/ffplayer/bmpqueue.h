#ifndef _BMP_QUEUE_H_
#define _BMP_QUEUE_H_

// ����ͷ�ļ�
#include <inttypes.h>
#include <semaphore.h>
#include "stdefine.h"

#ifdef __cplusplus
extern "C" {
#endif

// ��������
#define DEF_BMP_QUEUE_SIZE   3

typedef struct {
    long      head;
    long      tail;
    long      size;
    sem_t     semr;
    sem_t     semw;
    int64_t  *ppts;
    HBITMAP  *hbitmaps;
    BYTE    **pbmpbufs;
} BMPQUEUE;

// ��������
BOOL bmpqueue_create (BMPQUEUE *pbq, HDC hdc, int w, int h, int cdepth);
void bmpqueue_destroy(BMPQUEUE *pbq);
BOOL bmpqueue_isempty(BMPQUEUE *pbq);

//++ ���������ӿں������ڿ��п�д bitmap �Ĺ��� ++//
// bmpqueue_write_request ȡ�õ�ǰ��д�Ŀ��� bitmap
// bmpqueue_write_release �ͷŵ�ǰ��д�Ŀ��� bitmap
// bmpqueue_write_post ���д�����
// ����ʹ�÷�����
// bmpqueue_write_request ���᷵�� bitmap
// ��� bitmap ������
// ������ɹ�����ִ��
//     bmpqueue_write_post
// ������ʧ�ܣ���ִ��
//     bmpqueue_write_release
void bmpqueue_write_request(BMPQUEUE *pbq, int64_t **ppts, BYTE **pbuf, int *stride);
void bmpqueue_write_release(BMPQUEUE *pbq);
void bmpqueue_write_done   (BMPQUEUE *pbq);
//-- ���������ӿں������ڿ��п�д bitmap �Ĺ��� --//

//++ ���������ӿں������ڿ��пɶ� bitmap �Ĺ��� ++//
void bmpqueue_read_request(BMPQUEUE *pbq, int64_t **ppts, HBITMAP *hbitmap);
void bmpqueue_read_release(BMPQUEUE *pbq);
void bmpqueue_read_done   (BMPQUEUE *pbq);
//-- ���������ӿں������ڿ��пɶ� bitmap �Ĺ��� --//

#ifdef __cplusplus
}
#endif

#endif




