#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<mmsystem.h>
#pragma comment(lib , "winmm.lib")

WCHAR str[128] = { 0 };
const WCHAR* keyName[] =
{
L"",L"",L"",L"",L"",L"",L"",L"",

L"BACK",
L"TAB",
L"",

L"",L"",

L"RETURN",

L"",L"",

L"SHIFT",
L"CONTROL",
L"MENU",

L"",

L"CAPITAL",

L"",L"",L"",L"",L"",L"",

L"ESCAPE",

L"",L"",L"",L"",

L"SPACE",
L"",L"",
L"END",

L"",

L"LEFT",
L"UP",
L"RIGHT",
L"DOWN",

L"",L"",L"",L"",L"",

L"DELETE",

L"",

L"0",L"1",L"2",L"3",L"4",
L"5",L"6",L"7",L"8",L"9",

L"",L"",L"",L"",L"",L"",L"",

L"A",L"B",L"C",L"D",L"E",L"F",L"G",
L"H",L"I",L"J",L"K",L"L",L"M",L"N",
L"O",L"P",L"Q",L"R",L"S",L"T",
L"U",L"V",L"W",L"X",L"Y",L"Z",

L"",L"",L"",L"",L"",

L"NUMPAD0",
L"NUMPAD1",
L"NUMPAD2",
L"NUMPAD3",
L"NUMPAD4",
L"NUMPAD5",
L"NUMPAD6",
L"NUMPAD7",
L"NUMPAD8",
L"NUMPAD9",
L"MULTIPLY",
L"ADD",
L"SEPARATOR",
L"SUBTRACT",
L"DECIMAL",
L"DIVIDE",
L"F1",
L"F2",
L"F3",
L"F4",
L"F5",
L"F6",
L"F7",
L"F8",
L"F9",
L"F10",
L"F11",
L"F12",

L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",
L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",
L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",
L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",
L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",
L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",
L"",L"",

L"OEM_1",
L"OEM_PLUS",
L"OEM_COMMA",
L"OEM_MINUS",
L"OEM_PERIOD",
L"OEM_2",
L"OEM_3",

L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",
L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",
L"",L"",L"",L"",L"",L"",

L"OEM_4",
L"OEM_5",
L"OEM_6",
L"OEM_7" 
};

void load_wav(void)
{
	// 加载声音，

	int i = 0;
	for (i = 0; i < 223; i++)
	{
		if (keyName[i] != L"")
		{
			swprintf(str, L"open YANXI\\%s.wav alias %s", keyName[i], keyName[i]);
			mciSendStringW(str, 0, 0, 0);
		}
	}

	return;
}

class TIMER
{
public:
	clock_t _EndTime;
	clock_t _TimeInterval;

	TIMER(clock_t TimeInterval)
	{
		_TimeInterval = (TimeInterval > (clock_t)0) ? TimeInterval : (clock_t)0;
		_EndTime = clock() + TimeInterval;
		return;
	}
	bool isTimeCountEnd(void)
	{
		return (clock() >= _EndTime) ? true : false;
	}
	void ReTiming(void)
	{
		_EndTime = clock() + _TimeInterval;
		return;
	}
};

int state[] = { 0,31,32,33 };
int vKey[223] = { 0b000 };

void printKey(void)
{
#define _k(A)		"│ \033[%dm" A "\033[0m"
#define _g(A)		"┤ \033[%dm" A "\033[0m"
#define k(A)		"│\033[%dm" A "\033[0m"
#define m(A)		" \033[%dm" A "\033[0m "
#define s(v)		state[vKey[v]]
	printf("\033[2;0H"
		"┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐\n"
		k("Esc")"│   " _k("F1")_k("F2")_k("F3")_k("F4")"│ " _k("F5")_k("F6")_k("F7")_k("F8")"│ " _k("F9")k("F10")k("F11")k("F12")"│ " m("┌┐")m("┌┐")m("┌┐")"\n"
		"└───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ " m("└┘")m("└┘")m("└┘")"\n"
		"┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐ ┌───┬───┬───┬───┐\n"
		_k("~`")_k("!1")_k("@2")_k("#3")_k("$4")_k("%%5")_k("^6")_k("&7")_k("*8")_k("(9")_k(")0")_k("_-")_k("+=")_k("BacSp ")"│ " k("Ins")k("Hom")k("PUp")"│ " k("N L")_k("/ ")_k("* ")_k("- ")"│\n"
		"├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤ ├───┼───┼───┼───┤\n"
		_k("Tab ")_k("Q ")_k("W ")_k("E ")_k("R ")_k("T ")_k("Y ")_k("U ")_k("I ")_k("O ")_k("P ")_k("{[")_k("}]")_k("|\\  ")"│ " k("Del")k("End")k("PDn")"│ " _k("7 ")_k("8 ")_k("9 ")"│   │\n"
		"├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┘ ├───┼───┼───" _g("+ ") "│\n"
		_k("Caps ")_k("A ")_k("S ")_k("D ")_k("F ")_k("G ")_k("H ")_k("J ")_k("K ")_k("L ")_k(":;")_k("\"\'")_k("Enter<┘")"│               " _k("4 ")_k("5 ")_k("6 ")"│   │\n"
		"├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤     ┌───┐     ├───┼───┼───┼───┤\n"
		_k("Shift  ")_k("Z ")_k("X ")_k("C ")_k("V ")_k("B ")_k("N ")_k("M ")_k("<,")_k(">.")_k("?/")_k(" Shift   ")"│     " _k("↑")"│     " _k("1 ")_k("2 ")_k("3 ")"│   │\n"
		"├─────┬──┴─┬─┴──┬┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐ ├───┴───┼───" _g("E│")"│\n"
		_k("Ctrl")"│    " _k("Alt")_k("         Space        ")"│    " _k("Alt")"│    " k("Ctrl")"│ " _k("←")_k("↓")_k("→")"│ " _k("  0   ")_k(". ")_k("<┘")"│\n"
		"└─────┴────┴────┴───────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘ └───────┴───┴───┘\n"
		,s(VK_ESCAPE), s(VK_F1), s(VK_F2), s(VK_F3), s(VK_F4), s(VK_F5), s(VK_F6), s(VK_F7), s(VK_F8), s(VK_F9), s(VK_F10), s(VK_F11), s(VK_F12), s(VK_LBUTTON), s(VK_MBUTTON), s(VK_RBUTTON),
		s(VK_LBUTTON), s(VK_MBUTTON), s(VK_RBUTTON),
		s(VK_OEM_3), s('1'), s('2'), s('3'), s('4'), s('5'), s('6'), s('7'), s('8'), s('9'), s('0'), s(VK_OEM_MINUS), s(VK_OEM_PLUS), s(VK_BACK), s(VK_INSERT), s(VK_HOME), s(VK_PRIOR), s(VK_NUMLOCK), s(VK_DIVIDE), s(VK_MULTIPLY), s(VK_SUBTRACT),
		s(VK_TAB), s('Q'), s('W'), s('E'), s('R'), s('T'), s('Y'), s('U'), s('I'), s('O'), s('P'), s(VK_OEM_4), s(VK_OEM_6), s(VK_OEM_5), s(VK_DELETE), s(VK_END), s(VK_NEXT), s(VK_NUMPAD7), s(VK_NUMPAD8), s(VK_NUMPAD9),
		s(VK_ADD),
		s(VK_CAPITAL), s('A'), s('S'), s('D'), s('F'), s('G'), s('H'), s('J'), s('K'), s('L'), s(VK_OEM_1), s(VK_OEM_7), s(VK_RETURN), s(VK_NUMPAD4), s(VK_NUMPAD5), s(VK_NUMPAD6),
		s(VK_LSHIFT), s('Z'), s('X'), s('C'), s('V'), s('B'), s('N'), s('M'), s(VK_OEM_COMMA), s(VK_OEM_PERIOD), s(VK_OEM_2), s(VK_RSHIFT), s(VK_UP), s(VK_NUMPAD1), s(VK_NUMPAD2), s(VK_NUMPAD3),
		s(VK_RETURN),
		s(VK_LCONTROL), s(VK_LMENU), s(VK_SPACE), s(VK_RMENU), s(VK_RCONTROL), s(VK_LEFT), s(VK_DOWN), s(VK_RIGHT), s(VK_NUMPAD0), s(VK_DECIMAL), s(VK_RETURN)
	);
	/***
	 * ┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐
	 * │Esc│   │ F1│ F2│ F3│ F4│ │ F5│ F6│ F7│ F8│ │ F9│F10│F11│F12│  ┌┐  ┌┐  ┌┐
	 * └───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘  └┘  └┘  └┘
	 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐ ┌───┬───┬───┬───┐
	 * │~ `│! 1│@ 2│# 3│$ 4│% 5│^ 6│& 7│* 8│( 9│) 0│_ -│+ =│ BacSp │ │Ins│Hom│PUp│ │N L│ / │ * │ - │
	 * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤ ├───┼───┼───┼───┤
	 * │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │{ [│} ]│ | \ │ │Del│End│PDn│ │ 7 │ 8 │ 9 │   │
	 * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┘ ├───┼───┼───┤ + │
	 * │ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │: ;│" '│ Enter  │               │ 4 │ 5 │ 6 │   │
	 * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤     ┌───┐     ├───┼───┼───┼───┤
	 * │ Shift  │ Z │ X │ C │ V │ B │ N │ M │< ,│> .│? /│  Shift   │     │ ↑ │     │ 1 │ 2 │ 3 │   │
	 * ├─────┬──┴─┬─┴──┬┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐ ├───┴───┼───┤ E││
	 * │ Ctrl│    │Alt │         Space         │    │Alt │    │Ctrl│ │ ← │ ↓ │ → │ │   0   │ . │ <┘│
	 * └─────┴────┴────┴───────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘ └───────┴───┴───┘
	 */

	return;
}

int main(void)
{
	load_wav();

	system("cls");
	printf("\033[0m[松开] \033[31m[按下] \033[33m[按住] \033[32m[弹起]\033[0m");
	
	int i = 0;
	TIMER t(64);
	for (;;)
	{
		for (i = 1; i < 223; i++)
		{
			if (keyName[i] != L"")
			{
				if (vKey[i] == 0b001)
				{
					// 只有在[按下]的时候会播放声音

					swprintf(str, L"seek %s to start", keyName[i]);
					mciSendStringW(str, 0, 0, 0);

					swprintf(str, L"play %s", keyName[i]);
					mciSendStringW(str, 0, 0, 0);
				}
			}

			vKey[i] = (vKey[i] << 1) & 0b010;
		}

		for (;!t.isTimeCountEnd();)
		{
			for (i = 0; i < 223; i++)
			{
				vKey[i] |= (GetAsyncKeyState(i)) ? 0b001 : 0b000;
			}
		}
		t.ReTiming();

		printKey();
	}

	return 0;
}



