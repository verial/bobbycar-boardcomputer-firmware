#pragma once

#include "menudisplay.h"
#include "staticmenudefinition.h"
#include "utils.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "texts.h"
#include "displays/graphdisplay.h"
#include "statistics.h"

namespace {
class MainMenu;
class DualGraphDisplay;
}

namespace {
using GasGraphDisplay = makeComponent<GraphDisplay<1>, StaticText<TEXT_GAS>, MultiStatisticsSingleImpl, GasStatistics>;
using BremsGraphDisplay = makeComponent<GraphDisplay<1>, StaticText<TEXT_BREMS>, MultiStatisticsSingleImpl, BremsStatistics>;
using AvgSpeedGraphDisplay = makeComponent<GraphDisplay<1>, StaticText<TEXT_AVGSPEED>, MultiStatisticsSingleImpl, AvgSpeedStatistics>;
using AvgSpeedKmhGraphDisplay = makeComponent<GraphDisplay<1>, StaticText<TEXT_AVGSPEEDKMH>, MultiStatisticsSingleImpl, AvgSpeedKmhStatistics>;
using SumCurrentGraphDisplay = makeComponent<GraphDisplay<1>, StaticText<TEXT_SUMCURRENT>, MultiStatisticsSingleImpl, SumCurrentStatistics>;
using SumAbsoluteCurrentGraphDisplay = makeComponent<GraphDisplay<1>, StaticText<TEXT_SUMABSOLUTECURRENT>, MultiStatisticsSingleImpl, SumAbsoluteCurrentStatistics>;
using FrontVoltageGraphDisplay = makeComponent<GraphDisplay<1>, StaticText<TEXT_FRONTVOLTAGE>, MultiStatisticsSingleImpl, FrontVoltageStatistics>;
using BackVoltageGraphDisplay = makeComponent<GraphDisplay<1>, StaticText<TEXT_BACKVOLTAGE>, MultiStatisticsSingleImpl, BackVoltageStatistics>;
using BmsVoltageGraphDisplay = makeComponent<GraphDisplay<1>, StaticText<TEXT_BMSVOLTAGE>, MultiStatisticsSingleImpl, BmsVoltageStatistics>;
using BmsCurrentGraphDisplay = makeComponent<GraphDisplay<1>, StaticText<TEXT_BMSCURRENT>, MultiStatisticsSingleImpl, BmsCurrentStatistics>;
using BmsPowerGraphDisplay = makeComponent<GraphDisplay<1>, StaticText<TEXT_BMSPOWER>, MultiStatisticsSingleImpl, BmsPowerStatistics>;

class SumCurrentsComparisonStatistics : public virtual MultiStatisticsInterface<2>
{
    std::array<std::reference_wrapper<const statistics::ContainerType>, 2> getBuffers() const override
    {
        return {SumCurrentStatistics{}.getBuffer(), BmsCurrentStatistics{}.getBuffer()};
    }
};
using SumCurrentsComparisonGraphDisplay = makeComponent<GraphDisplay<2>, StaticText<TEXT_SUMCURRENTSCOMPARISON>, SumCurrentsComparisonStatistics>;

class MotorCurrentsStatistics : public virtual MultiStatisticsInterface<4>
{
    std::array<std::reference_wrapper<const statistics::ContainerType>, 4> getBuffers() const override
    {
        return {FrontLeftCurrentStatistics{}.getBuffer(), FrontRightCurrentStatistics{}.getBuffer(), BackLeftCurrentStatistics{}.getBuffer(), BackRightCurrentStatistics{}.getBuffer()};
    }
};
using MotorCurrentsGraphDisplay = makeComponent<GraphDisplay<4>, StaticText<TEXT_MOTORCURRENTS>, MotorCurrentsStatistics>;

class GraphsMenu :
    public MenuDisplay,
    public StaticText<TEXT_GRAPHS>,
    public StaticMenuDefinition<
        makeComponent<MenuItem, StaticText<TEXT_GAS>,                   SwitchScreenAction<GasGraphDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_BREMS>,                 SwitchScreenAction<BremsGraphDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_AVGSPEED>,              SwitchScreenAction<AvgSpeedGraphDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_AVGSPEEDKMH>,           SwitchScreenAction<AvgSpeedKmhGraphDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_SUMCURRENT>,            SwitchScreenAction<SumCurrentGraphDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_SUMABSOLUTECURRENT>,    SwitchScreenAction<SumAbsoluteCurrentGraphDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_FRONTVOLTAGE>,          SwitchScreenAction<FrontVoltageGraphDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_BACKVOLTAGE>,           SwitchScreenAction<BackVoltageGraphDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_BMSVOLTAGE>,            SwitchScreenAction<BmsVoltageGraphDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_BMSCURRENT>,            SwitchScreenAction<BmsCurrentGraphDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_BMSPOWER>,              SwitchScreenAction<BmsPowerGraphDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_SUMCURRENTSCOMPARISON>, SwitchScreenAction<SumCurrentsComparisonGraphDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_MOTORCURRENTS>,         SwitchScreenAction<MotorCurrentsGraphDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_DUALGRAPHS>,            SwitchScreenAction<DualGraphDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_BACK>,                  SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&icons::back>>
    >
{};
}