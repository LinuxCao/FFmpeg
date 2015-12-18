#ifndef _WAV_QUEUE_H_
#define _WAV_QUEUE_H_

// ����ͷ�ļ�
#include <inttypes.h>
#include <semaphore.h>
#include "stdefine.h"

#ifdef __cplusplus
extern "C" {
#endif

// ��������
#define DEF_WAV_QUEUE_SIZE   5
#define DEF_WAV_BUFFER_SIZE  8192

typedef struct {
    long      head;
    long      tail;
    long      size;
    sem_t     semr;
    sem_t     semw;
    int64_t  *ppts;
    WAVEHDR  *pwhdrs;
    void     *adev;
} WAVQUEUE;

// ��������
BOOL wavqueue_create (WAVQUEUE *pwq, void *adev, int wavbufsize);
void wavqueue_destroy(WAVQUEUE *pwq);
BOOL wavqueue_isempty(WAVQUEUE *pwq);

//++ ���������ӿں������ڿ��п�д wavehdr �Ĺ��� ++//
// wavqueue_write_request ȡ�õ�ǰ��д�Ŀ��� wavehdr
// wavqueue_write_release �ͷŵ�ǰ��д�Ŀ��� wavehdr
// wavqueue_write_post ���д�����
// ����ʹ�÷�����
// wavqueue_write_request ���᷵�� wavehdr
// ��� wavehdr ������
// ������ɹ�����ִ��
//     wavqueue_write_post
// ������ʧ�ܣ���ִ��
//     wavqueue_write_release
void wavqueue_write_request(WAVQUEUE *pwq, int64_t **ppts, PWAVEHDR *pwhdr);
void wavqueue_write_release(WAVQUEUE *pwq);
void wavqueue_write_done   (WAVQUEUE *pwq);
//-- ���������ӿں������ڿ��п�д wavehdr �Ĺ��� --//

//++ ���������ӿں������ڿ��пɶ� wavehdr �Ĺ��� ++//
void wavqueue_read_request(WAVQUEUE *pwq, int64_t **ppts, PWAVEHDR *pwhdr);
void wavqueue_read_release(WAVQUEUE *pwq);
void wavqueue_read_done   (WAVQUEUE *pwq);
//-- ���������ӿں������ڿ��пɶ� wavehdr �Ĺ��� --//

#ifdef __cplusplus
}
#endif

#endif




