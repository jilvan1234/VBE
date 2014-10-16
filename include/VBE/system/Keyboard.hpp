#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

///
/// @brief Provides interface for a keyboard device
///
class Keyboard {
	public:
		///
		/// \brief Contains all supported keys
		///
		enum Key {
			Num0,
			Num1,
			Num2,
			Num3,
			Num4,
			Num5,
			Num6,
			Num7,
			Num8,
			Num9,
			A,
			AC_BACK,
			AC_BOOKMARKS,
			AC_FORWARD,
			AC_HOME,
			AC_REFRESH,
			AC_SEARCH,
			AC_STOP,
			Again,
			AltErase,
			Quote,
			Application,
			AudioMute,
			AudioNext,
			AudioPlay,
			AudioPrev,
			AuidoStop,
			B,
			Backslash,
			Backspace,
			BrightnessDown,
			BrightnessUp,
			C,
			Calculator,
			Cancel,
			Capslock,
			Clear,
			ClearAgain,
			Comma,
			Computer,
			Copy,
			CrSel,
			CurrencySubUnit,
			CurrencyUnit,
			Cut,
			D,
			DecimalSeparator,
			Delete,
			DisplaySwitch,
			Down,
			E,
			Eject,
			End,
			Equals,
			Escape,
			Execute,
			Exsel,
			F,
			F1,
			F10,
			F11,
			F12,
			F13,
			F14,
			F15,
			F16,
			F17,
			F18,
			F19,
			F2,
			F20,
			F21,
			F22,
			F23,
			F24,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			Find,
			G,
			BackQuote,
			H,
			Help,
			Home,
			I,
			Insert,
			J,
			K,
			KBDIllumDown,
			KBDIllumToggle,
			KBDIllumUp,
			Keypad0,
			Keypad00,
			Keypad000,
			Keypad1,
			Keypad2,
			Keypad3,
			Keypad4,
			Keypad5,
			Keypad6,
			Keypad7,
			Keypad8,
			Keypad9,
			KeypadA,
			KeypadAmpersand,
			KeypadAt,
			KeypadB,
			KeypadBackspace,
			KeypadBinary,
			KeypadC,
			KeypadClear,
			KeypadClearEntry,
			KeypadColon,
			KeypadComma,
			KeypadD,
			KeypadDoubleAmpersand,
			KeypadDoubleVerticalBar,
			KeypadDecimal,
			KeypadDivide,
			KeypadE,
			KeypadEnter,
			KeypadEquals,
			KeypadEqualsAS400,
			KeypadExclamation,
			KeypadF,
			KeypadGreater,
			KeypadHash,
			KeypadHexadecimal,
			KeypadLBrace,
			KeypadLParenthesis,
			KeypadLess,
			KeypadMemAdd,
			KeypadMemClear,
			KeypadMemDivide,
			KeypadMemMultiply,
			KeypadMemRecall,
			KeypadMemStore,
			KeypadMemSubstract,
			KeypadMinus,
			KeypadMultiply,
			KeypadOctal,
			KeypadPercent,
			KeypadPeriod,
			KeypadPlus,
			KeypadPlusMinus,
			KeypadPower,
			KeypadRBrace,
			KeypadRParenthesis,
			KeypadSpace,
			KeypadTab,
			KeypadVerticalBar,
			KeypadXor,
			L,
			LAlt,
			LControl,
			Left,
			LBracket,
			LGUI,
			LShift,
			M,
			Mail,
			MediaSelect,
			Menu,
			Minus,
			Mode,
			Mute,
			N,
			NumLockClear,
			O,
			Oper,
			Out,
			P,
			PageDown,
			PageUp,
			Paste,
			Pause,
			Period,
			Power,
			PrintScren,
			Prior,
			Q,
			R,
			RAlt,
			RControl,
			Return,
			Return2,
			RGUI,
			Right,
			RBracket,
			RShift,
			S,
			ScrollLock,
			Select,
			Semicolont,
			Separator,
			Slash,
			Sleep,
			Space,
			Stop,
			Sysreq,
			T,
			Tab,
			ThousandsSeparator,
			U,
			Undo,
			Unknown,
			UP,
			V,
			VolumeDown,
			VolumeUp,
			W,
			WWW,
			X,
			Y,
			Z,
			Ampersand,
			Asterisk,
			At,
			Caret,
			Colon,
			Dollar,
			Exclamation,
			Greater,
			Hash,
			LParenthesis,
			Less,
			Percent,
			Plus,
			Question,
			DoubleQuote,
			RParenthesis,
			Underscore,
		};

		///
		/// \brief Tells wether the requested key is pressed (held down)
		///
		static bool pressed(Key k);

		///
		/// \brief Tells wether the requested key is just released (It was not pressed on the last frame and now it is.)
		///
		static bool justPressed(Key k);

		///
		/// \brief Tells wether the requested key is just released (It was pressed on the last frame and it's now released.)
		///
		static bool justReleased(Key k);
};

///
/// @class Keyboard Keyboard.hpp "enviroment/Keyboard.hpp"
/// @ingroup System
///
/// You can use this class within an init Environment to access the current
/// keyboard device's state. Not all keys are supported in all devices and will
/// never be marked as pressed. The state of this device will be updated to
/// match recieved events whenever Environment::update() is called.
///
/// A Key will just be pressed for one frame (one update()) call. Then held for
/// an indefinite number of frames and released right after. For Example, if the
/// user pressed the A Key on frame 1 and released it on frame 4, this would
/// register (updating the environment every frame of course):
///
/// - Frame 1
///   + Key Keyboard::A is pressed
///   + Key Keyboard::A is held
///   + Key Keyboard::A is not released
/// - Frame 2
///   + Key Keyboard::A is not pressed
///   + Key Keyboard::A is held
///   + Key Keyboard::A is not released
/// - Frame 3
///   + Key Keyboard::A is not pressed
///   + Key Keyboard::A is held
///   + Key Keyboard::A is not released
/// - Frame 4
///   + Key Keyboard::A is not pressed
///   + Key Keyboard::A is not held
///   + Key Keyboard::A is released
///
/// @see Environment

#endif // KEYBOARD_HPP