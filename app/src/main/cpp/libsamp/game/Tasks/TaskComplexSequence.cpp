
#include "TaskComplexSequence.h"
void CTaskComplexSequence::InjectHooks() {

}

// 0x632BD0
CTaskComplexSequence::CTaskComplexSequence() : CTaskComplex() {
    m_nCurrentTaskIndex = 0;
    m_bRepeatSequence = false;
    m_nSequenceRepeatedCount = 0;
    m_bFlushTasks = false;
    m_nReferenceCount = 0;
    //std::ranges::fill(m_aTasks, nullptr);
    memset(m_aTasks, 0, sizeof(CTaskComplexSequence::m_aTasks));
}

// 0x6389F0
CTaskComplexSequence::~CTaskComplexSequence() {
    Flush();
}

// 0x632C10
void CTaskComplexSequence::Flush() {
    for (auto& task : m_aTasks) {
        delete task;
        task = nullptr;
    }

    m_nCurrentTaskIndex = 0;
    m_bRepeatSequence = false;
    m_nSequenceRepeatedCount = 0;
}

CTaskComplexSequence* CTaskComplexSequence::Constructor() {
    CTaskComplexSequence();
    return this;
}

// 0x5F6710
CTask* CTaskComplexSequence::Clone() {
    return Clone_Reversed();
}

// 0x632C00
bool CTaskComplexSequence::MakeAbortable(CPed* ped, eAbortPriority priority, const CEvent* event) {
    return MakeAbortable_Reversed(ped, priority, event);
}

// 0x638A40
CTask* CTaskComplexSequence::CreateNextSubTask(CPed* ped) {
    return CreateNextSubTask_Reversed(ped);
}

// 0x638A60
CTask* CTaskComplexSequence::CreateFirstSubTask(CPed* ped) {
    return CreateFirstSubTask_Reversed(ped);
}

// 0x632D00
CTask* CTaskComplexSequence::ControlSubTask(CPed* ped) {
    return ControlSubTask_Reversed(ped);
}

CTask* CTaskComplexSequence::Clone_Reversed() {
    auto* sequence = new CTaskComplexSequence();
    for (auto taskIndex = 0u; taskIndex < std::size(m_aTasks); taskIndex++) {
        CTask* task = m_aTasks[taskIndex];
        sequence->m_aTasks[taskIndex] = task ? task->Clone() : nullptr;
    }
    sequence->m_bRepeatSequence   = m_bRepeatSequence;
    sequence->m_nCurrentTaskIndex = m_nCurrentTaskIndex;
    return sequence;
}

bool CTaskComplexSequence::MakeAbortable_Reversed(CPed* ped, eAbortPriority priority, const CEvent* event) {
    return m_pSubTask->MakeAbortable(ped, priority, event);
}

CTask* CTaskComplexSequence::CreateNextSubTask_Reversed(CPed* ped) {
    return CreateNextSubTask(ped, m_nCurrentTaskIndex, m_nSequenceRepeatedCount);
}

CTask* CTaskComplexSequence::CreateFirstSubTask_Reversed(CPed* ped) {
    CTask* currentTask = m_aTasks[m_nCurrentTaskIndex];
    return currentTask ? currentTask->Clone() : nullptr;
}

CTask* CTaskComplexSequence::ControlSubTask_Reversed(CPed* ped) {
    return m_pSubTask;
}

// 0x632D10
void CTaskComplexSequence::AddTask(CTask* task) {
    for (auto& m_aTask : m_aTasks) {
        if (!m_aTask) {
            m_aTask = task;
            return;
        }
    }

    delete task;
    Log("Failed to add task to sequence");
}

// 0x632D50
void CTaskComplexSequence::AddTask(int32 sequenceIdx, CTask* task) {
    if (sequenceIdx >= (int32)std::size(m_aTasks)) {
        delete task;
        Log("Failed to add task to sequence");
    } else {
        delete m_aTasks[sequenceIdx];
        m_aTasks[sequenceIdx] = task;
    }
}

// 0x632C70
CTask* CTaskComplexSequence::CreateNextSubTask(CPed* ped, int32& taskIndex, int32& repeatCount) {
  //  UNUSED(ped);

    CTask* nextSubTask = nullptr;
    int32 incrementedTaskIndex = taskIndex + 1;
    taskIndex = incrementedTaskIndex;

    if (m_bRepeatSequence) {
        if (incrementedTaskIndex == std::size(m_aTasks) || !m_aTasks[incrementedTaskIndex]) {
            taskIndex = 0;
            repeatCount = repeatCount + 1;
        }

        if (m_bRepeatSequence || (bool)repeatCount != m_bRepeatSequence) {
            nextSubTask = m_aTasks[taskIndex]->Clone();
        }
    } else if (incrementedTaskIndex != std::size(m_aTasks)) {
        if (CTask* task = m_aTasks[incrementedTaskIndex]) {
            nextSubTask = task->Clone();
        }
    }
    return nextSubTask;
}

// 0x41BF10
bool CTaskComplexSequence::Contains(eTaskType taskType) {
    for (auto& task : m_aTasks) {
        if (task->GetTaskType() == taskType) {
            return true;
        }
    }
    return false;
}

// 0x463610
void CTaskComplexSequence::f0x463610(bool flush) {
    if (m_nReferenceCount || !flush) {
        m_bFlushTasks = flush;
    } else {
        m_bFlushTasks = false;
        Flush();
    }
}

// 0x636BC0
void CTaskComplexSequence::f0x636BC0() {
    if (m_nReferenceCount-- == 1 && m_bFlushTasks) {
        m_bFlushTasks = false;
        Flush();
    }
}
