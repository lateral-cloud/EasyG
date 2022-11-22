#pragma once

#include "EasyGraphDef.h"

namespace EasyGraph {
	class EasyMessage {
	public:
		UINT message;
		union {
			struct
			{
				bool ctrl : 1;
				bool shift : 1;
				bool lbutton : 1;
				bool mbutton : 1;
				bool rbutton : 1;
				short wheel;
				short x;
				short y;
			};

			struct
			{
				BYTE vkcode;
				BYTE scancode;
				bool extended : 1;
				bool prevdown : 1;
			};

			TCHAR ch;

			struct
			{
				WPARAM wParam;
				LPARAM lParam;
			};
		};
	};
}