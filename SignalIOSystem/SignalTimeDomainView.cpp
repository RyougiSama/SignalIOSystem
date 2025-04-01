#include "SignalTimeDomainView.h"

SignalTimeDomainView::SignalTimeDomainView(QWidget *parent)
    : QChartView(parent)
{

}

SignalTimeDomainView::~SignalTimeDomainView()
{
}

void SignalTimeDomainView::loadSignalData(SignalModel::SignalFileType file_t)
{
	switch (file_t) {
	case SignalModel::NONE:
		break;
	case SignalModel::LOAD_FROM_DATA:
		this->sample_rate = this->model->get_signal_sample_rate();
		this->disp_waveform = this->model->get_signal_raw_data();
		break;
	case SignalModel::LOAD_FROM_CONFIG:
		break;
	default:
		break;
	}
}

// Update function
void SignalTimeDomainView::updateChartView()
{

}

