/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/utils/taskrunner/TaskProcessor.h"
#include "cdm/utils/taskrunner/TaskQueue.h"
#include "cdm/utils/taskrunner/Task.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Constructor
///
/// \param  pTaskQueue  Pointer to the TaskQueue to draw Tasks from
//--------------------------------------------------------------------------------------------------
TaskProcessor::TaskProcessor(TaskQueue* pTaskQueue)
    : m_pTaskQueue(pTaskQueue)
{
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Parenthesis operator that draws and runs Tasks until the TaskQueue is empty
//--------------------------------------------------------------------------------------------------
void TaskProcessor::operator()()
{
    while (std::unique_ptr<Task> pTask = m_pTaskQueue->GetNextTask())
    {
        pTask->Run();
        m_pTaskQueue->ReportTaskComplete();
    }
}