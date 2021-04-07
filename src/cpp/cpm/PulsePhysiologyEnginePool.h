
#include "stdafx.h"
#include "CommonDataModel.h"
#include "Engine.h"

#include "ThreadPool.h"

class PulsePhysiologyEnginePool
{
public:
    PulsePhysiologyEnginePool(size_t engineNum, size_t poolSize = std::thread::hardware_concurrency());

    std::vector<bool> initAll(std::vector<SEPatientConfiguration*> configurations);
    std::vector<bool> initAll(std::vector<std::string> patientFiles);
    std::vector<bool> advanceAll(double time, TimeUnit unit);

    const std::vector<std::pair<std::unique_ptr<PulseEngine>, bool>>& getEngines();

    const size_t workerCount() const { return m_pool.workerCount(); }

private:
    std::vector<std::pair<std::unique_ptr<PulseEngine>, bool>> m_engines;
    std::vector<bool> m_isWorking;
    ThreadPool m_pool;
};