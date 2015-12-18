// ����ͷ�ļ�
#include "wavqueue.h"

// ����ʵ��
BOOL wavqueue_create(WAVQUEUE *pwq, void *adev, int wavbufsize)
{
    BYTE *pwavbuf;
    int   i;

    // default size
    if (pwq->size  == 0) pwq->size  = DEF_WAV_QUEUE_SIZE;
    if (wavbufsize == 0) wavbufsize = DEF_WAV_BUFFER_SIZE;

    // alloc buffer & semaphore
    pwq->ppts    = (int64_t*)malloc(pwq->size * sizeof(int64_t));
    pwq->pwhdrs  = (WAVEHDR*)malloc(pwq->size * (sizeof(WAVEHDR) + wavbufsize));
    sem_init(&(pwq->semr), 0, 0        );
    sem_init(&(pwq->semw), 0, pwq->size);
    pwq->adev = adev;

    // check invalid
    if (!pwq->ppts || !pwq->pwhdrs) {
        wavqueue_destroy(pwq);
        return FALSE;
    }

    // clear
    memset(pwq->ppts  , 0, pwq->size * sizeof(int64_t));
    memset(pwq->pwhdrs, 0, pwq->size * sizeof(WAVEHDR));

    // init
    pwavbuf = (BYTE*)(pwq->pwhdrs + pwq->size);
    for (i=0; i<pwq->size; i++) {
        pwq->pwhdrs[i].lpData         = (LPSTR)(pwavbuf + i * wavbufsize);
        pwq->pwhdrs[i].dwBufferLength = wavbufsize;
        pwq->pwhdrs[i].dwUser         = wavbufsize;
        waveOutPrepareHeader((HWAVEOUT)pwq->adev, &(pwq->pwhdrs[i]), sizeof(WAVEHDR));
    }
    return TRUE;
}

void wavqueue_destroy(WAVQUEUE *pwq)
{
    int i;

    // unprepare
    for (i=0; i<pwq->size; i++) {
        waveOutUnprepareHeader((HWAVEOUT)pwq->adev, &(pwq->pwhdrs[i]), sizeof(WAVEHDR));
    }

    if (pwq->ppts  ) free(pwq->ppts  );
    if (pwq->pwhdrs) free(pwq->pwhdrs);
    sem_destroy(&(pwq->semr));
    sem_destroy(&(pwq->semw));

    // clear members
    memset(pwq, 0, sizeof(WAVQUEUE));
}

BOOL wavqueue_isempty(WAVQUEUE *pwq)
{
    int value = 0;
    sem_getvalue(&(pwq->semr), &value);
    return (value <= 0);
}

void wavqueue_write_request(WAVQUEUE *pwq, int64_t **ppts, PWAVEHDR *pwhdr)
{
    sem_wait(&(pwq->semw));
    if (ppts ) *ppts  = &(pwq->ppts[pwq->tail]);
    if (pwhdr) *pwhdr = &(pwq->pwhdrs[pwq->tail]);
}

void wavqueue_write_release(WAVQUEUE *pwq)
{
    sem_post(&(pwq->semw));
}

void wavqueue_write_done(WAVQUEUE *pwq)
{
    if (++pwq->tail == pwq->size) pwq->tail = 0;
    sem_post(&(pwq->semr));
}

void wavqueue_read_request(WAVQUEUE *pwq, int64_t **ppts, PWAVEHDR *pwhdr)
{
    sem_wait(&(pwq->semr));
    if (ppts ) *ppts  = &(pwq->ppts[pwq->head]);
    if (pwhdr) *pwhdr = &(pwq->pwhdrs[pwq->head]);
}

void wavqueue_read_release(WAVQUEUE *pwq)
{
    sem_post(&(pwq->semr));
}

void wavqueue_read_done(WAVQUEUE *pwq)
{
    if (++pwq->head == pwq->size) pwq->head = 0;
    sem_post(&(pwq->semw));
}





