#ifndef GS_INPUT_H

enum key_code
{
    KeyCode_Invalid,
    
    KeyCode_Space,
    KeyCode_Tab,
    KeyCode_CapsLock,
    KeyCode_LeftShift, KeyCode_RightShift,
    KeyCode_LeftCtrl, KeyCode_RightCtrl,
    KeyCode_Fn, 
    KeyCode_Alt, 
    KeyCode_PageUp, KeyCode_PageDown,
    KeyCode_Backspace, KeyCode_Delete,
    KeyCode_Enter,
    
    // Function Keys
    KeyCode_F0, KeyCode_F1, KeyCode_F2, KeyCode_F3, KeyCode_F4, KeyCode_F5, KeyCode_F6, KeyCode_F7,
    KeyCode_F8, KeyCode_F9, KeyCode_F10, KeyCode_F11, KeyCode_F12, 
    
    // Letters
    KeyCode_a, KeyCode_b, KeyCode_c, KeyCode_d, KeyCode_e, KeyCode_f, KeyCode_g, KeyCode_h,
    KeyCode_i, KeyCode_j, KeyCode_k, KeyCode_l, KeyCode_m, KeyCode_n, KeyCode_o, KeyCode_p, 
    KeyCode_q, KeyCode_r, KeyCode_s, KeyCode_t, KeyCode_u, KeyCode_v, KeyCode_w, KeyCode_x, 
    KeyCode_y, KeyCode_z,
    
    KeyCode_A, KeyCode_B, KeyCode_C, KeyCode_D, KeyCode_E, KeyCode_F, KeyCode_G, KeyCode_H,
    KeyCode_I, KeyCode_J, KeyCode_K, KeyCode_L, KeyCode_M, KeyCode_N, KeyCode_O, KeyCode_P, 
    KeyCode_Q, KeyCode_R, KeyCode_S, KeyCode_T, KeyCode_U, KeyCode_V, KeyCode_W, KeyCode_X, 
    KeyCode_Y, KeyCode_Z,
    
    // Numbers
    KeyCode_0, KeyCode_1, KeyCode_2, KeyCode_3, KeyCode_4, KeyCode_5, KeyCode_6, KeyCode_7,
    KeyCode_8, KeyCode_9,
    
    KeyCode_Num0, KeyCode_Num1, KeyCode_Num2, KeyCode_Num3, KeyCode_Num4, KeyCode_Num5, 
    KeyCode_Num6, KeyCode_Num7, KeyCode_Num8, KeyCode_Num9,
    
    // Symbols
    KeyCode_Bang, KeyCode_At, KeyCode_Pound, KeyCode_Dollar, KeyCode_Percent, KeyCode_Carrot,
    KeyCode_Ampersand, KeyCode_Star, KeyCode_LeftParen, KeyCode_RightParen, KeyCode_Minus, KeyCode_Plus,
    KeyCode_Equals, KeyCode_Underscore, KeyCode_LeftBrace, KeyCode_RightBrace, KeyCode_LeftBracket,
    KeyCode_RightBracket, KeyCode_Colon, KeyCode_SemiColon, KeyCode_SingleQuote, KeyCode_DoubleQuote,
    KeyCode_ForwardSlash, KeyCode_Backslash, KeyCode_Pipe, KeyCode_Comma, KeyCode_Period, 
    KeyCode_QuestionMark, KeyCode_LessThan, KeyCode_GreaterThan, KeyCode_Tilde, KeyCode_BackQuote,
    
    // Arrows
    KeyCode_UpArrow,
    KeyCode_DownArrow,
    KeyCode_LeftArrow,
    KeyCode_RightArrow,
    
    // Mouse
    // NOTE(Peter): Including this here so we can utilize the same KeyDown, KeyUp etc. functions
    KeyCode_MouseLeftButton,
    KeyCode_MouseMiddleButton,
    KeyCode_MouseRightButton,
    
    KeyCode_Count,
};

#define GS_INPUT_H
#endif