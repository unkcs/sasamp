#pragma once

#include "TaskComplex.h"
#include "../common.h"

class CTask;

class CTaskComplexSequence : public CTaskComplex {
public:
    int32    m_nCurrentTaskIndex;      // Used in m_aTasks
    CTask*   m_aTasks[8];
    bool     m_bRepeatSequence;        // Sequence will loop if set to 1
    uint8_t  pad0[3];
    int32    m_nSequenceRepeatedCount; // m_nSequenceRepeatedCount simply tells us how many times the sequence has been repeated.
    // If m_bRepeatSequence is true, this can be greater than 1,
    // otherwise it's set to 1 when the sequence is done executing tasks.
    bool     m_bFlushTasks;
    uint8_t  pad1[3];
    uint32   m_nReferenceCount; // count of how many CTaskComplexUseSequence instances are using this sequence

public:
    static constexpr auto Type = TASK_COMPLEX_SEQUENCE;

    /*!
    * Construct using multiple tasks, same as constructing
    * and then calling `AddTask` for every task passed in.
    */
    template<Task... T>
    CTaskComplexSequence(T*... tasks) :
            CTaskComplexSequence{}
    {
        (AddTask(tasks), ...);
    }

    CTaskComplexSequence();
    ~CTaskComplexSequence() override;

    eTaskType GetTaskType() override { return Type; } // 0x632C60
    CTask* Clone() override;
    bool MakeAbortable(class CPedGta* ped, eAbortPriority priority, const CEvent* event) override;
    CTask* CreateNextSubTask(CPedGta* ped) override;
    CTask* CreateFirstSubTask(CPedGta* ped) override;
    CTask* ControlSubTask(CPedGta* ped) override;

    void AddTask(CTask* task);
    void AddTask(int32 id, CTask* task);
    CTask* CreateNextSubTask(CPedGta* ped, int32& taskIndex, int32& repeatCount);
    void Flush();
    bool Contains(eTaskType taskType);
    void f0x463610(bool flush);
    void f0x636BC0();

private:
    friend void InjectHooksMain();
    static void InjectHooks();

    CTaskComplexSequence* Constructor();

    CTask* Clone_Reversed();
    bool MakeAbortable_Reversed(class CPedGta* ped, eAbortPriority priority, const CEvent* event);
    CTask* CreateNextSubTask_Reversed(CPedGta* ped);
    CTask* CreateFirstSubTask_Reversed(CPedGta* ped);
    CTask* ControlSubTask_Reversed(CPedGta* ped);
};
VALIDATE_SIZE(CTaskComplexSequence, 0x40);
