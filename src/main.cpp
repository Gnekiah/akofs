#include <iostream>
#include <string>
#include <spk_compat.h>
#include <spk_setproctitle.h>

static void usage() 
{
    std::cerr << "usage: sparkle -i "
        << ""
        << std::endl;
}

int main(int argc, const char **argv) 
{
    spk_setproctitle(argv, "sparkle");
    spk_pthread_setname(pthread_self(), "sparkle");
    int err = 0;

    // ���������в���
    // ���ز����������ļ�
    // ��ʼ����־��¼��
    // ��ʼ������ģ��
    // ��ʼ���¼�����ģ��
    // ��ʼ����ʱ��
    // ��ʼ������IO
    // ��ʼ����ϢЭ��ģ��
    // ��ʼ��KV�־û��洢
    // ��ʼ��������־
    // ��ʼ�����ݽڵ����������ڵ���������ע���¼�����ص�����������ʱ��ע�ᶨʱ�¼���
    // ��ʼ�������ڵ���� ... ��
    // ��ʼ�����ʽڵ���� ... ��
    // �ӽڵ��ļ����Իָ��ڵ㣨���ݽڵ��/�򳬼��ڵ㣩
    // ������Ϣѭ��

    return 0;
}