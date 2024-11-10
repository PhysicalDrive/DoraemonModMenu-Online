#pragma once 
#include "../../Common.hpp"

namespace Chim
{
	class LocalFeatures
	{
	public:
		void Tick()
		{
			SuperMan(m_SuperMan);
			NinjaJump(m_NinjaJump);
			FastRun(m_FastRun);
			FastSwim(m_FastSwim);
			NeverWanted(m_NeverWanted);
			GodMode(m_GodMode);
			DrugMode(m_DrugMode);
			Tiny(m_Tiny);
			MONEYDROP(m_moneydrop);
			RPDROP(m_rpdrop);
			Ignore(m_ignore);
		}

		bool m_SuperMan = false;
		bool m_SuperJump = false;
		bool m_BeastJump = false;
		bool m_NinjaJump = false;
		bool m_GracefulLanding = false;
		bool m_FastRun = false;
		bool m_FastSwim = false;
		bool m_GodMode = false;
		bool m_NeverWanted = false;
		bool m_DrugMode = false;
		bool m_Tiny = false;
		bool m_moneydrop = false;
		bool m_rpdrop = false;
		bool m_ignore = false;

	private:
		void SuperMan(bool enable);
		void NinjaJump(bool enable);
		void FastRun(bool enable);
		void FastSwim(bool enable);
		void GodMode(bool enable);
		void NeverWanted(bool enable);
		void DrugMode(bool enable);
		void Tiny(bool enable);

		// money
		void MONEYDROP(bool enable);
		void RPDROP(bool enable);
		void Ignore(bool enable);
	};

	inline std::unique_ptr<LocalFeatures> g_LocalFeatures;
}