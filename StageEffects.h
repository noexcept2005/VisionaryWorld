//VisionaryWorld ר��ͷ�ļ�
//�κ��˲��õ��á�
//Written by Rico
#pragma once
	typedef UINT EFFECT_ID;

#define STAGE_NONE 0x0000		//��
#define STAGE_FADEIN 0x0001		//����
#define STAGE_FADEOUT 0x0002	//����

class StageEffect {		//��̨Ч�����ƹ⣬��ĻЧ��֮�ࣩ
	public:

		EFFECT_ID id;
		bool active;

		clock_t start;		//START TIMESTAMP
		DWORD duration;		//UNIT: ms

		color_t color;

		StageEffect(EFFECT_ID _id, color_t _clr)
		{
			id = _id, color = _clr;
			start = clock();
			active = true;
		}
		StageEffect()
		{
			start = clock();
			active = true;
		}
		bool IsAlive()
		{
			if (duration <= 0)
				return false;
			return ((clock() - start) <= duration);
		}
		bool IsActive()
		{
			return active;
		}
		void SetActive(bool _active)
		{
			active = _active;
		}

		void Draw()
		{
			if (!IsActive() || !IsAlive())	return;

			if (id == STAGE_FADEIN || id == STAGE_FADEOUT)
			{
				clock_t delta = clock() - start;
				if (delta < 0)
					return;
				double ratio = (delta) / (double)duration;
				//DebugLog(ToString(ratio));
				if (ratio > 1)
					return;

				PIMAGE scr = newimage();
				getimage(scr, 0, 0, uix, uiy);	//��ȡ������Ļ
				//����/�� Ч��
				for (int i = 0; i < uiy; i++)
					for (int j = 0; j < uix; j++)
					{
						color_t pix = getpixel_f(j, i, scr);	//��ȡ����
						color_t fnl;
						if (id == STAGE_FADEIN)	//color ->  Ӧ�е�����
							fnl = EgeColor(StepColor(WinColor(color), WinColor(pix), ratio));	//����
						else if (id == STAGE_FADEOUT)	//ԭ�������� -> color
							fnl = EgeColor(StepColor(WinColor(pix), WinColor(color), ratio));	//����
						putpixel_f(j, i, fnl, scr);			//����ԭ��������
					}
				putimage(0, 0, scr);	//������Ļ
				//swappage();
				delay_fps(60);
			}
			else {
				DebugLog("��ERROR����Ч����̨Ч��: id=" + ToString(id));
				return;
			}
		}

	};
	vector<StageEffect> stageEffects{};

	void DrawStageEffects()
	{
		if (stageEffects.empty())	return;

		for (int i = 0; i < stageEffects.size(); ++i)
			stageEffects.at(i).Draw();
	}

	void AddStageEffect(EFFECT_ID id, color_t color, DWORD duration)	//�����̨Ч��
	{
		StageEffect eff;
		eff.id = id;
		eff.color = color;
		eff.start = clock();
		eff.duration = duration;
		eff.active = true;

		stageEffects.push_back(eff);
	}

	bool NoStageEffectsAlive()
	{
		for (int i = 0; i < stageEffects.size(); i++)
			if (stageEffects.at(i).IsAlive())
				return false;
		return true;
	}

	void CheckStageEffects()
	{
		for (int i = 0; i < stageEffects.size(); i++)
		{
			if (!stageEffects.at(i).IsAlive())
			{
				int index = i;
				vector<StageEffect>::iterator it = stageEffects.begin();	//������ 
				while (it != stageEffects.begin() + index && it != stageEffects.end())
					it++;
				it = stageEffects.erase(it);	//ɾ�� 
				//DebugLog("Erased StageEffect, index = " + ToString(index));
				continue;
			}
		}
	}

	void ClearStageEffects()
	{
		stageEffects.clear();
	}
