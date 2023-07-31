ScriptName BakaAutoLock extends ScriptObject Native

; @brief Returns the API version.
; @return Returns 0 if not installed, else returns the API version.
int Function GetVersion() Native Global

; @brief Display messagebox with lock roll modifiers.
Function ShowRollModifiers() Native Global

; @brief Display messagebox with hack roll modifiers.
Function ShowRollModifiersHack() Native Global

; @brief Update values from MCM settings files.
Function UpdateSettings() Native Global
