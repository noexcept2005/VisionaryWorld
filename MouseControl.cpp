//Written by Ricochesta 


void RefreshPrecur()
{
	int _left=   ;
	int _top=    ;
	int* precur = &g_precur;
	int _x = (g_mx - _left) / g_slot_px;
	int _y = (g_my - _top) / g_slot_px;
	if(_y > 2)
	{
		if(g_my > _top + g_slot_px * 3 + GAP && _top + g_slot_px * 4 + GAP)
			_y = 0;
		else
		{
			precur = -1;
			return ;
		}
	}else if( _y < 0)
	{
		precur = -1;
		return;
	}
		
	precur = (_y + 1) * 10 + _x;
	
}
DWORD WINAPI ThrMouseControl(LPVOID none) 
{
	//g_mx,g_my mean the pos of cursor
	
	RefreshPrecur();
}
