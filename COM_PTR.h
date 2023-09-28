#pragma once
// COM_PRT�N���X

// DirectX�̃C���^�[�t�F�C�X�̎Q�ƃJ�E���^��R�s�[�Ȃǂ̖�����������B
// �f�o�b�O���ɂ̓C���^�[�t�F�C�X�̎g�p�󋵂�DEBUG.txt�Ƃ����t�@�C������
// �O���ɏo�͂��܂��B
// �f�o�b�O���ɂ��̋@�\��OFF�ɂ���ɂ͉���
// OUTPUT_INTERFACEUSAGE�萔���R�����g�A�E�g���Ă��������B

#define OUTPUT_INTERFACEUSAGE

#ifdef OUTPUT_INTERFACEUSAGE
#if _DEBUG
// �f�o�b�O���Ƀt�@�C���o��
#include <fstream>
using namespace std;
#endif
#endif

template <class T>
class COM_PRT
{
private:
    T* m_pInterface;   // �C���^�[�t�F�C�X�ւ̃|�C���^
protected:
    // �Q�ƃJ�E���^����
    void AddRef(T* pInterface) 
    {
#if _DEBUG
        // �f�o�b�O�łł͎Q�ƃJ�E���g���o�͂���
        ULONG c = pInterface->AddRef();
        ofstream ofs;
        ofs.open("DEBUG.txt", ios::app);
        ofs << pInterface << "\t[" << c << "]" << endl;
        ofs.close();
#else
        pInterface->AddRef();
#endif
    }
    // �Q�ƃJ�E���^����
    void Release(T* pInterface) 
    {
#if _DEBUG
        ULONG c = m_pInterface->Release();
        ofstream ofs;
        ofs.open("DEBUG.txt", ios::app);
        ofs << m_pInterface << "\t[" << c << "]" << endl;
        ofs.close();
#else
        m_pInterface->Release();
#endif
    }
public:
    // �f�t�H���g�R���X�g���N�^
    // �R���X�g���N�^��explicit�錾���܂��B
    // �����func(COM_PRT<IHoge> v)�Ƃ����֐��ɑ΂��āA
    // func(pIHoge)�̂悤�ȈÖٌ^�ϊ�����͏o���܂���B
    // �ʏ�A�|�C���^�̈��n�����ɂ��łɎQ�ƃJ�E���^��
    // �C���N�������g����Ă���̂ŁAAddRef�͍s���܂���B
    explicit COM_PRT(T* pInterface = NULL, BOOL add = FALSE) 
    {
        if (pInterface && add) {
            AddRef(pInterface);
        }
        m_pInterface = pInterface;
    }
    // �R�s�[�R���X�g���N�^
    COM_PRT(const COM_PRT& src) 
    {
        // �R�s�[���̎Q�ƃJ�E���^��1���₷
        if (src.m_pInterface)
            AddRef(src.m_pInterface);

        // �R�s�[
        m_pInterface = src.m_pInterface;
    }
    // �f�X�g���N�^
    virtual ~COM_PRT() 
    {
        if (m_pInterface)
            Release(m_pInterface);
    }
    // ���Z�q
    // =������Z�q�i�����I�R�s�[�j
    COM_PRT& operator =(const COM_PRT& src) 
    {
        // �����̃C���^�[�t�F�C�X�̎Q�ƃJ�E���^��1���炷
        if (m_pInterface)
            Release(m_pInterface);

        // �R�s�[���̎Q�ƃJ�E���^��1���₷
        if (src.m_pInterface)
            AddRef(src.m_pInterface);

        // �R�s�[
        m_pInterface = src.m_pInterface;

        return *this;
    }
    // =������Z�q�i�C���^�[�t�F�C�X�o�^�j
    void operator =(T* pInterface) 
    {
        // �����̃C���^�[�t�F�C�X�̎Q�ƃJ�E���^��1���炷
        if (m_pInterface)
            Release(m_pInterface);

        // �R�s�[
        m_pInterface = pInterface;
    }
    // !=��r���Z�q
    BOOL operator !=(int val) 
    {
        if (val != (int)m_pInterface)
            return TRUE;
        return FALSE;
    }
    // ==��r���Z�q
    BOOL operator ==(int val) 
    {
        if (val == (int)m_pInterface)
            return TRUE;
        return FALSE;
    }
    // �����o�֐�
    // �|�C���^�擾
    T* getPtr() { return m_pInterface; }
    // �|�C���^�̃A�h���X���擾
    T** getPtrPtr() { return &m_pInterface; }
    // �C���^�[�t�F�C�X�����֐��֓n����p�֐�
    T** toCreator() 
    {
        if (m_pInterface)
            m_pInterface->Release();

        return &m_pInterface;
    }
};
