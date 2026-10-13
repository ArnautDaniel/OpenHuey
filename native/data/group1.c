/* The compiler-generated part of the game's data, for the native build (hg): the static
 * constructor table, the vtables and the pointer-to-member constants, as written by
 * tools/data_c.py from the split executable. Each run is the words of a stretch of adjacent
 * labels at their PS2 offsets; each label is an alias into it (see tools/data_c.py). Not
 * compiled into the PS2 build, which keeps the disassembly. */

/* label `n` at offset `o` of run `a` (C names are "_name" in 32-bit Windows objects; COFF has no .type) */
#ifdef _WIN32
#define G1_ALIAS(n, a, o) ".globl _" n "\n.set _" n ", _" a " + " o "\n"
#else
#define G1_ALIAS(n, a, o) ".globl " n "\n.type " n ", @object\n.set " n ", " a " + " o "\n"
#endif

extern char Actor_CanAct[];
extern char Actor_Cleanup[];
extern char Actor_FilesLoaded[];
extern char Actor_FilesLoading[];
extern char Actor_LeaveScreen[];
extern char Actor_LightChange[];
extern char Actor_LoadFiles[];
extern char Actor_Place[];
extern char Actor_RememberPos[];
extern char Actor_Reset[];
extern char Actor_ShowUp[];
extern char Actor_Unload[];
extern char Actor_Update[];
extern char Actor_dtor[];
extern char AdxBase_dtor[];
extern char AshFlakes_Draw[];
extern char AshFlakes_Start[];
extern char AshFlakes_Update[];
extern char AshFlakes_dtor[];
extern char AvoidPromptBase_dtor[];
extern char AvoidPrompt_DrawPart[];
extern char AvoidPrompt_Show[];
extern char AvoidPrompt_dtor[];
extern char BackdropModel2_Draw[];
extern char BackdropModel2_SetParams[];
extern char BackdropModel2_Start[];
extern char BackdropModel2_Update[];
extern char BackdropModel2_dtor[];
extern char BackdropModel_Draw[];
extern char BackdropModel_Start[];
extern char BackdropModel_Update[];
extern char BackdropModel_dtor[];
extern char BadAlloc_What[];
extern char BadException_What[];
extern char Ball_Draw[];
extern char Ball_Flying[];
extern char Ball_Frame[];
extern char Ball_LandTri[];
extern char Ball_MotionState[];
extern char Ball_PutDown[];
extern char Ball_Setup[];
extern char Ball_StateDropping[];
extern char Ball_StateRolling[];
extern char Ball_dtor[];
extern char BgmCtl_Want[];
extern char BgmCtl_dtor[];
extern char BigFire_Draw[];
extern char BigFire_SetParams[];
extern char BigFire_Start[];
extern char BigFire_Update[];
extern char BigFire_dtor[];
extern char BitMask_Clear[];
extern char BlockPool_Alloc[];
extern char BlockPool_Free[];
extern char BlockPool_FreeAll[];
extern char BlockPool_dtor[];
extern char BloodSpray_Draw[];
extern char BloodSpray_SetParams[];
extern char BloodSpray_Start[];
extern char BloodSpray_Update[];
extern char BloodSpray_dtor[];
extern char Bloom_Draw[];
extern char Bloom_dtor[];
extern char BoneHangPoint_Frame[];
extern char BonePoint_Loaded[];
extern char BonePoint_dtor[];
extern char BoneSmoke_Draw[];
extern char BoneSmoke_SetParams[];
extern char BoneSmoke_Start[];
extern char BoneSmoke_Update[];
extern char BoneSmoke_dtor[];
extern char BootCard_dtor[];
extern char BurstA_Draw[];
extern char BurstA_SetParams[];
extern char BurstA_Start[];
extern char BurstA_Update[];
extern char BurstA_dtor[];
extern char BurstB_Draw[];
extern char BurstB_SetParams[];
extern char BurstB_Start[];
extern char BurstB_Update[];
extern char BurstB_dtor[];
extern char Butterflies_Draw[];
extern char Butterflies_SetParams[];
extern char Butterflies_Start[];
extern char Butterflies_Update[];
extern char Butterflies_dtor[];
extern char CamDirectorBase_dtor[];
extern char CamDirector_Follow[];
extern char CamDirector_FreeMode[];
extern char CamDirector_HoldEffect1C[];
extern char CamDirector_ModeDefault[];
extern char CamDirector_ModeEvent[];
extern char CamDirector_ModeNormal[];
extern char CamDirector_NextSetup[];
extern char CamDirector_PathLength[];
extern char CamDirector_PathSeek[];
extern char CamDirector_Remember[];
extern char CamDirector_Restart[];
extern char CamDirector_Set50[];
extern char CamDirector_SetSetup[];
extern char CamDirector_Setup[];
extern char CamDirector_SetupChanged[];
extern char CamDirector_dtor[];
extern char CameraBase_dtor[];
extern char Camera_ApplyPreset[];
extern char Camera_ApplyPresetArg[];
extern char Camera_CurrentPreset[];
extern char Camera_Defaults[];
extern char Camera_FollowPath[];
extern char Camera_Get294[];
extern char Camera_Get298[];
extern char Camera_Get60[];
extern char Camera_GetDir[];
extern char Camera_GetEye[];
extern char Camera_GetFar[];
extern char Camera_GetFov[];
extern char Camera_GetMtx150[];
extern char Camera_GetMtx190[];
extern char Camera_GetMtx1D0[];
extern char Camera_GetMtx210[];
extern char Camera_GetNear[];
extern char Camera_GetPath[];
extern char Camera_GetRoll[];
extern char Camera_GetTarget[];
extern char Camera_GetView[];
extern char Camera_GetViewScreen[];
extern char Camera_MoveEye[];
extern char Camera_MoveLocal[];
extern char Camera_MoveTarget[];
extern char Camera_MtxTranslation[];
extern char Camera_NoPath[];
extern char Camera_Orbit[];
extern char Camera_OutOfView[];
extern char Camera_PathCount[];
extern char Camera_Preset[];
extern char Camera_Reset[];
extern char Camera_Set28[];
extern char Camera_Set4[];
extern char Camera_SetCentre[];
extern char Camera_SetEye[];
extern char Camera_SetFar[];
extern char Camera_SetFov[];
extern char Camera_SetMtx150[];
extern char Camera_SetMtx190[];
extern char Camera_SetNear[];
extern char Camera_SetRoll[];
extern char Camera_SetTarget[];
extern char Camera_SetZMax[];
extern char Camera_SetZMin[];
extern char Camera_TurnView[];
extern char Camera_Update[];
extern char Camera_ViewAxis[];
extern char Camera_ViewHeading[];
extern char Camera_ViewMatrix[];
extern char Camera_ZValue[];
extern char Camera_dtor[];
extern char Capsule_Push[];
extern char Capsule_Update[];
extern char Caustic_Draw[];
extern char Caustic_dtor[];
extern char CeilingDrips_Draw[];
extern char CeilingDrips_Start[];
extern char CeilingDrips_Update[];
extern char CeilingDrips_dtor[];
extern char CharModel_ClearDraw[];
extern char CharModel_Frame[];
extern char CharModel_Loaded[];
extern char CharModel_MarkerFile[];
extern char CharModel_SecondaryMotion[];
extern char CharModel_SetFlag[];
extern char CharModel_SetPoint[];
extern char CharModel_SetVector[];
extern char CharModel_ShowParts[];
extern char CharModel_VtCC[];
extern char CharModel_VtD0[];
extern char CharModel_dtor[];
extern char Character_Activate[];
extern char Character_AttackPoint[];
extern char Character_BackToNormal[];
extern char Character_BackToStance[];
extern char Character_Deactivate[];
extern char Character_Disable[];
extern char Character_Enable[];
extern char Character_EventCommand[];
extern char Character_EventConcerns[];
extern char Character_EventReset[];
extern char Character_IsBusy[];
extern char Character_LeftBehind[];
extern char Character_LoadMessage[];
extern char Character_LoadState[];
extern char Character_Place[];
extern char Character_PlaceModel[];
extern char Character_RegionFade[];
extern char Character_RememberPos[];
extern char Character_Reset[];
extern char Character_ResetBehaviour[];
extern char Character_SaveState[];
extern char Character_Think[];
extern char Character_TimerDown[];
extern char Character_dtor[];
extern char Costume2Model_BufferSize[];
extern char Costume2Model_Loaded[];
extern char Costume2Model_ModelFile[];
extern char Costume2Model_MotionTable[];
extern char Costume2Model_Part0[];
extern char Costume2Model_Part1[];
extern char Costume2Model_Part2[];
extern char Costume2Model_Part3[];
extern char Costume2Model_Part4[];
extern char Costume2Model_SetPoint[];
extern char Costume2Model_ShowParts[];
extern char Costume2Model_Springs[];
extern char Costume2Model_Textures[];
extern char Costume2Model_Textures2[];
extern char Costume2Model_VtCC[];
extern char Costume2Model_VtD0[];
extern char Costume2Model_dtor[];
extern char Costume3Model_BufferSize[];
extern char Costume3Model_Loaded[];
extern char Costume3Model_ModelFile[];
extern char Costume3Model_MotionTable[];
extern char Costume3Model_Part0[];
extern char Costume3Model_Part1[];
extern char Costume3Model_Part2[];
extern char Costume3Model_Part3[];
extern char Costume3Model_Part4[];
extern char Costume3Model_SetPoint[];
extern char Costume3Model_Springs[];
extern char Costume3Model_Textures[];
extern char Costume3Model_Textures2[];
extern char Costume3Model_VtCC[];
extern char Costume3Model_dtor[];
extern char Costume6Model_BufferSize[];
extern char Costume6Model_Loaded[];
extern char Costume6Model_MarkerFile[];
extern char Costume6Model_ModelFile[];
extern char Costume6Model_MotionTable[];
extern char Costume6Model_Part0[];
extern char Costume6Model_Part1[];
extern char Costume6Model_Part2[];
extern char Costume6Model_Part3[];
extern char Costume6Model_Part4[];
extern char Costume6Model_SetFlag[];
extern char Costume6Model_SetPoint[];
extern char Costume6Model_Springs[];
extern char Costume6Model_Textures[];
extern char Costume6Model_Textures2[];
extern char Costume6Model_Vt2C[];
extern char Costume6Model_Vt30[];
extern char Costume6Model_dtor[];
extern char Costume7Model_BufferSize[];
extern char Costume7Model_Loaded[];
extern char Costume7Model_MarkerFile[];
extern char Costume7Model_ModelFile[];
extern char Costume7Model_MotionTable[];
extern char Costume7Model_Part0[];
extern char Costume7Model_Part1[];
extern char Costume7Model_Part2[];
extern char Costume7Model_Part3[];
extern char Costume7Model_Part4[];
extern char Costume7Model_SetPoint[];
extern char Costume7Model_Springs[];
extern char Costume7Model_Textures[];
extern char Costume7Model_Textures2[];
extern char Costume7Model_Vt2C[];
extern char Costume7Model_Vt30[];
extern char Costume7Model_dtor[];
extern char Costume8Model_BufferSize[];
extern char Costume8Model_Loaded[];
extern char Costume8Model_ModelFile[];
extern char Costume8Model_MotionTable[];
extern char Costume8Model_Part0[];
extern char Costume8Model_Part1[];
extern char Costume8Model_Part2[];
extern char Costume8Model_Part3[];
extern char Costume8Model_Part4[];
extern char Costume8Model_Textures[];
extern char Costume8Model_Textures2[];
extern char Costume8Model_dtor[];
extern char CostumeHangPoint_Frame[];
extern char Costumes_State[];
extern char Cr19Bubbles_Draw[];
extern char Cr19Bubbles_SetParams[];
extern char Cr19Bubbles_Start[];
extern char Cr19Bubbles_Update[];
extern char Cr19Bubbles_dtor[];
extern char CreatureA_ActionState[];
extern char CreatureA_Activate[];
extern char CreatureA_Cleanup[];
extern char CreatureA_ComeAfterFiona[];
extern char CreatureA_Draw[];
extern char CreatureA_EnterRoom[];
extern char CreatureA_FilesLoaded[];
extern char CreatureA_FionaLeft[];
extern char CreatureA_Frame[];
extern char CreatureA_IsUp[];
extern char CreatureA_ModelMatrix[];
extern char CreatureA_Place[];
extern char CreatureA_PlaceInRoom[];
extern char CreatureA_Save[];
extern char CreatureA_SetSaveSlot[];
extern char CreatureA_Setup[];
extern char CreatureA_SetupKind[];
extern char CreatureA_StateAfterFiona[];
extern char CreatureA_StateHangAbout[];
extern char CreatureA_StateNone[];
extern char CreatureA_StateRoute[];
extern char CreatureA_StateToDoor[];
extern char CreatureA_StateVanish[];
extern char CreatureA_Unload[];
extern char CreatureA_Vt88[];
extern char CreatureA_dtor[];
extern char CreatureB_ActionState[];
extern char CreatureB_Activate[];
extern char CreatureB_Cleanup[];
extern char CreatureB_ComeAfterFiona[];
extern char CreatureB_Draw[];
extern char CreatureB_DrawLight[];
extern char CreatureB_EnterRoom[];
extern char CreatureB_FilesLoaded[];
extern char CreatureB_FionaLeft[];
extern char CreatureB_Frame[];
extern char CreatureB_IsUp[];
extern char CreatureB_ModelMatrix[];
extern char CreatureB_Place[];
extern char CreatureB_PlaceInRoom[];
extern char CreatureB_Save[];
extern char CreatureB_SetSaveSlot[];
extern char CreatureB_Setup[];
extern char CreatureB_SetupKind[];
extern char CreatureB_StateAfterFiona[];
extern char CreatureB_StateApproach10[];
extern char CreatureB_StateByProgress[];
extern char CreatureB_StateDoor[];
extern char CreatureB_StateFionaCaught[];
extern char CreatureB_StateFionaInSight[];
extern char CreatureB_StateFionaOffMesh[];
extern char CreatureB_StateFlag9[];
extern char CreatureB_StateGrab[];
extern char CreatureB_StateHeldOff[];
extern char CreatureB_StateHit[];
extern char CreatureB_StateKnockedDown[];
extern char CreatureB_StateLeaving[];
extern char CreatureB_StateNone[];
extern char CreatureB_StateRoute[];
extern char CreatureB_StateToDoor[];
extern char CreatureB_StateTurnToFiona[];
extern char CreatureB_StateWalkSpot[];
extern char CreatureB_StateWatch0[];
extern char CreatureB_StateWatch1[];
extern char CreatureB_Unload[];
extern char CreatureB_Vt88[];
extern char CreatureB_dtor[];
extern char CreatureBase_Cleanup[];
extern char CreatureBase_ComeAfterFiona[];
extern char CreatureBase_Draw[];
extern char CreatureBase_EnterRoom[];
extern char CreatureBase_FionaLeft[];
extern char CreatureBase_Frame[];
extern char CreatureBase_IsUp[];
extern char CreatureBase_ModelMatrix[];
extern char CreatureBase_Save[];
extern char CreatureBase_SetSaveSlot[];
extern char CreatureBase_Setup[];
extern char CreatureBase_SetupKind[];
extern char CreatureBase_dtor[];
extern char CreatureVanish_Draw[];
extern char CreatureVanish_SetParams[];
extern char CreatureVanish_Start[];
extern char CreatureVanish_Update[];
extern char CreatureVanish_dtor[];
extern char Creatures_Call2C[];
extern char Creatures_Get[];
extern char Creatures_Keep[];
extern char Creatures_MessageSlot[];
extern char Creatures_ModelCall[];
extern char Creatures_ModelSet[];
extern char Creatures_PoolCall[];
extern char Creatures_Remove[];
extern char Creatures_SendAfterFiona[];
extern char Creatures_SetUpOne[];
extern char Cutscene_Call8[];
extern char Cutscene_ClearStatus[];
extern char Cutscene_Destroy[];
extern char Cutscene_End[];
extern char Cutscene_EntryDone[];
extern char Cutscene_Frame[];
extern char Cutscene_FrameSignals[];
extern char Cutscene_Get206[];
extern char Cutscene_Get216[];
extern char Cutscene_Get28[];
extern char Cutscene_IntoShot[];
extern char Cutscene_Letterbox[];
extern char Cutscene_Mark82[];
extern char Cutscene_Mark83[];
extern char Cutscene_NearEnd[];
extern char Cutscene_NextEvent[];
extern char Cutscene_NextShot[];
extern char Cutscene_PushC[];
extern char Cutscene_RunState[];
extern char Cutscene_Set38[];
extern char Cutscene_SetHold[];
extern char Cutscene_SetLetterboxOff[];
extern char Cutscene_SetNoEnd[];
extern char Cutscene_SetScript[];
extern char Cutscene_ShotAt[];
extern char Cutscene_Signal2[];
extern char Cutscene_SignalBits[];
extern char Cutscene_SignalCount[];
extern char Cutscene_Signals34[];
extern char Cutscene_Slot84[];
extern char Cutscene_SlotGroup[];
extern char Cutscene_Start[];
extern char Cutscene_StateFirstShot[];
extern char Cutscene_StatePlaying[];
extern char Cutscene_StateScript[];
extern char Cutscene_Status[];
extern char Cutscene_Update[];
extern char D_0044C890[];
extern char D_0044C8E8[];
extern char D_0044CAA8[];
extern char D_0044CAE0[];
extern char D_0044CB08[];
extern char D_0044CB38[];
extern char D_0044CBE8[];
extern char D_0044CBF0[];
extern char D_0044CC50[];
extern char D_0044CCB0[];
extern char D_0044CCF8[];
extern char D_0044CD90[];
extern char D_0044CDA0[];
extern char D_0044CE00[];
extern char D_0044CE58[];
extern char D_0044CE98[];
extern char D_0044CF30[];
extern char D_0044CF70[];
extern char D_0044CFD0[];
extern char D_0044D098[];
extern char D_0044D0A0[];
extern char D_0044D0F0[];
extern char D_0044D220[];
extern char D_004543E0[];
extern char DaniellaModel_Frame[];
extern char DaniellaModel_Loaded[];
extern char DaniellaModel_Part0[];
extern char DaniellaModel_Part1[];
extern char DaniellaModel_Part2[];
extern char DaniellaModel_Part3[];
extern char DaniellaModel_SecondaryMotion[];
extern char DaniellaModel_Vt2C[];
extern char DaniellaModel_Vt30[];
extern char DaniellaModel_Vt3C[];
extern char DaniellaModel_dtor[];
extern char Daniella_ActionOffsets[];
extern char Daniella_AttackAnimA[];
extern char Daniella_AttackAnimB[];
extern char Daniella_AttackTable[];
extern char Daniella_BonePositions[];
extern char Daniella_DoorBreak[];
extern char Daniella_DoorOffset[];
extern char Daniella_ExitArg[];
extern char Daniella_FilesLoaded[];
extern char Daniella_FrightSeen[];
extern char Daniella_Kind[];
extern char Daniella_ModelFiles[];
extern char Daniella_MotionFiles[];
extern char Daniella_ReachHewie[];
extern char Daniella_SetRage[];
extern char Daniella_Setup[];
extern char Daniella_ThreatAmount[];
extern char Daniella_Update[];
extern char Daniella_dtor[];
extern char Debilitas2_ActionOffsets[];
extern char Debilitas2_AttackAngle[];
extern char Debilitas2_AttackAnimA[];
extern char Debilitas2_AttackAnimB[];
extern char Debilitas2_AttackRange[];
extern char Debilitas2_AttackTable[];
extern char Debilitas2_BlockFlags[];
extern char Debilitas2_CarryOn[];
extern char Debilitas2_ChaseTarget[];
extern char Debilitas2_Dist2E8[];
extern char Debilitas2_DoorAnim[];
extern char Debilitas2_DoorOffset[];
extern char Debilitas2_ExitDone[];
extern char Debilitas2_FreshStart[];
extern char Debilitas2_GivesUp[];
extern char Debilitas2_GoForFiona[];
extern char Debilitas2_GoTo[];
extern char Debilitas2_HeadFor[];
extern char Debilitas2_HeadForFiona[];
extern char Debilitas2_HeadingStep[];
extern char Debilitas2_LookFrames[];
extern char Debilitas2_LookSwing[];
extern char Debilitas2_ModelFiles[];
extern char Debilitas2_MotionFiles[];
extern char Debilitas2_PickDestination[];
extern char Debilitas2_ReachFiona[];
extern char Debilitas2_ReachHewie[];
extern char Debilitas2_RoomSpots[];
extern char Debilitas2_SetTimer[];
extern char Debilitas2_Setup[];
extern char Debilitas2_SlowWalkAnim[];
extern char Debilitas2_SpeedBase[];
extern char Debilitas2_SpeedTop[];
extern char Debilitas2_Stairs[];
extern char Debilitas2_StartGrab[];
extern char Debilitas2_StartLunge[];
extern char Debilitas2_StartTurnToFiona[];
extern char Debilitas2_StateBlow[];
extern char Debilitas2_StateGrab[];
extern char Debilitas2_StateLunge[];
extern char Debilitas2_StateStun[];
extern char Debilitas2_StateTurnToFiona[];
extern char Debilitas2_Timer10s[];
extern char Debilitas2_TurnRate[];
extern char Debilitas2_TurnRateFast[];
extern char Debilitas2_Update[];
extern char Debilitas2_dtor[];
extern char Debilitas3_ActionOffsets[];
extern char Debilitas3_Activate[];
extern char Debilitas3_Arrived[];
extern char Debilitas3_AttackAngle[];
extern char Debilitas3_AttackAnimA[];
extern char Debilitas3_AttackAnimB[];
extern char Debilitas3_AttackRange[];
extern char Debilitas3_AttackTable[];
extern char Debilitas3_BlockFlags[];
extern char Debilitas3_ChaseTarget[];
extern char Debilitas3_Dist2E8[];
extern char Debilitas3_DoorAnim[];
extern char Debilitas3_DoorOffset[];
extern char Debilitas3_ExitDone[];
extern char Debilitas3_FollowPathExit[];
extern char Debilitas3_GoTo[];
extern char Debilitas3_HeadFor[];
extern char Debilitas3_HeadForFiona[];
extern char Debilitas3_LookFrames[];
extern char Debilitas3_LookSwing[];
extern char Debilitas3_ModelFiles[];
extern char Debilitas3_MotionFiles[];
extern char Debilitas3_OnToNextExit[];
extern char Debilitas3_PickDestination[];
extern char Debilitas3_ReachFiona[];
extern char Debilitas3_ReachHewie[];
extern char Debilitas3_SetTimer[];
extern char Debilitas3_Setup[];
extern char Debilitas3_SpeedBase[];
extern char Debilitas3_SpeedTop[];
extern char Debilitas3_Stairs[];
extern char Debilitas3_StartGrab[];
extern char Debilitas3_StartLunge[];
extern char Debilitas3_StartTurnToFiona[];
extern char Debilitas3_StateBlow[];
extern char Debilitas3_StateGrab[];
extern char Debilitas3_StateLunge[];
extern char Debilitas3_StateNone[];
extern char Debilitas3_StateStun[];
extern char Debilitas3_StateTurnToFiona[];
extern char Debilitas3_Timer10s[];
extern char Debilitas3_TurnRate[];
extern char Debilitas3_TurnRateFast[];
extern char Debilitas3_Update[];
extern char Debilitas3_WalkToExit[];
extern char Debilitas3_dtor[];
extern char DebilitasModel_Frame[];
extern char DebilitasModel_Loaded[];
extern char DebilitasModel_Part0[];
extern char DebilitasModel_Part1[];
extern char DebilitasModel_Part2[];
extern char DebilitasModel_Part3[];
extern char DebilitasModel_SecondaryMotion[];
extern char DebilitasModel_Vt3C[];
extern char DebilitasModel_dtor[];
extern char Debilitas_ActionOffsets[];
extern char Debilitas_AttackAngle[];
extern char Debilitas_AttackAnimA[];
extern char Debilitas_AttackAnimB[];
extern char Debilitas_AttackRange[];
extern char Debilitas_AttackTable[];
extern char Debilitas_Behaviour[];
extern char Debilitas_BehaviourRun[];
extern char Debilitas_BlockFlags[];
extern char Debilitas_CarryOn[];
extern char Debilitas_Chase[];
extern char Debilitas_ChaseDecision[];
extern char Debilitas_ChaseTarget[];
extern char Debilitas_Dist2E8[];
extern char Debilitas_DoorAnim[];
extern char Debilitas_DoorOffset[];
extern char Debilitas_ExitDone[];
extern char Debilitas_FreshStart[];
extern char Debilitas_GivesUp[];
extern char Debilitas_GoForFiona[];
extern char Debilitas_GoTo[];
extern char Debilitas_HeadFor[];
extern char Debilitas_HeadForFiona[];
extern char Debilitas_HeadingStep[];
extern char Debilitas_LookFrames[];
extern char Debilitas_LookSwing[];
extern char Debilitas_ModelFiles[];
extern char Debilitas_MotionFiles[];
extern char Debilitas_PickDestination[];
extern char Debilitas_ReachFiona[];
extern char Debilitas_ReachHewie[];
extern char Debilitas_RoomSpots[];
extern char Debilitas_SetTimer[];
extern char Debilitas_Setup[];
extern char Debilitas_SlowWalkAnim[];
extern char Debilitas_SpeedBase[];
extern char Debilitas_SpeedTop[];
extern char Debilitas_Stairs[];
extern char Debilitas_StandAnim[];
extern char Debilitas_StartGrab[];
extern char Debilitas_StartLunge[];
extern char Debilitas_StartTurnToFiona[];
extern char Debilitas_StartWander[];
extern char Debilitas_StateBlow[];
extern char Debilitas_StateGrab[];
extern char Debilitas_StateLookWalk[];
extern char Debilitas_StateLunge[];
extern char Debilitas_StateStartWalk[];
extern char Debilitas_StateStun[];
extern char Debilitas_StateTurnToFiona[];
extern char Debilitas_Timer10s[];
extern char Debilitas_TurnRate[];
extern char Debilitas_TurnRateFast[];
extern char Debilitas_Update[];
extern char Debilitas_dtor[];
extern char Debris_Draw[];
extern char Debris_SetParams[];
extern char Debris_Start[];
extern char Debris_Update[];
extern char Debris_dtor[];
extern char DepthRange_Draw[];
extern char DepthRange_SetParams[];
extern char DepthRange_Start[];
extern char DepthRange_Update[];
extern char DepthRange_dtor[];
extern char DofDrawer_dtor[];
extern char DogModelA_KindTable[];
extern char DogModelA_Table[];
extern char DogModelA_dtor[];
extern char DogModelB_KindTable[];
extern char DogModelB_Table[];
extern char DogModelB_dtor[];
extern char DogModel_AdjustBone[];
extern char DogModel_BodyFrames[];
extern char DogModel_FeetContact[];
extern char DogModel_FeetReplace[];
extern char DogModel_FeetUp[];
extern char DogModel_FloorLevel[];
extern char DogModel_FootDown[];
extern char DogModel_Frame[];
extern char DogModel_HalfHip[];
extern char DogModel_HeadBone[];
extern char DogModel_HeadPos[];
extern char DogModel_Loaded[];
extern char DogModel_MarkerFile[];
extern char DogModel_ModelFile[];
extern char DogModel_Part0[];
extern char DogModel_Part1[];
extern char DogModel_Part2[];
extern char DogModel_Part3[];
extern char DogModel_PlantFeet[];
extern char DogModel_SecondaryMotion[];
extern char DogModel_SetMatrix[];
extern char DogModel_Textures[];
extern char DogModel_dtor[];
extern char DoorShadow_Draw[];
extern char DoorShadow_dtor[];
extern char DoorsBase_dtor[];
extern char Doors_AnimUserSpot[];
extern char Doors_AtDoor[];
extern char Doors_DataBuffer[];
extern char Doors_GetNavTri[];
extern char Doors_GetOpenAngle[];
extern char Doors_GetPoint20[];
extern char Doors_GetPos[];
extern char Doors_GetRestTurn[];
extern char Doors_HasExit[];
extern char Doors_InArea[];
extern char Doors_IsIdle[];
extern char Doors_IsOpened[];
extern char Doors_OnSide[];
extern char Doors_OpenFromNear[];
extern char Doors_OpenSide[];
extern char Doors_Refresh[];
extern char Doors_RoomIn[];
extern char Doors_SaveAll[];
extern char Doors_SaveDoor[];
extern char Doors_SelectBuffer[];
extern char Doors_SetBits[];
extern char Doors_SetFlag82[];
extern char Doors_SetOpened[];
extern char Doors_ShutSide[];
extern char Doors_Side[];
extern char Doors_SpotAhead[];
extern char Doors_SpotBehind[];
extern char Doors_SpotJustBehind[];
extern char Doors_StartAnim[];
extern char Doors_TakeSection8[];
extern char Doors_TurnBy[];
extern char Doors_TurnTo[];
extern char Doors_dtor[];
extern char DriftingFlecks_Draw[];
extern char DriftingFlecks_SetParams[];
extern char DriftingFlecks_Start[];
extern char DriftingFlecks_Update[];
extern char DriftingFlecks_dtor[];
extern char Drips_Draw[];
extern char Drips_Start[];
extern char Drips_Update[];
extern char Drips_dtor[];
extern char DropletFlash_Draw[];
extern char DropletFlash_SetParams[];
extern char DropletFlash_Start[];
extern char DropletFlash_Update[];
extern char DropletFlash_dtor[];
extern char DropletSpray_Draw[];
extern char DropletSpray_SetParams[];
extern char DropletSpray_Start[];
extern char DropletSpray_Update[];
extern char DropletSpray_dtor[];
extern char DustMoteSource_Draw[];
extern char DustMoteSource_SetParams[];
extern char DustMoteSource_Start[];
extern char DustMoteSource_Update[];
extern char DustMoteSource_dtor[];
extern char DustMote_Draw[];
extern char DustMote_SetParams[];
extern char DustMote_Start[];
extern char DustMote_Update[];
extern char DustMote_dtor[];
extern char DustShaft_Draw[];
extern char DustShaft_Start[];
extern char DustShaft_Update[];
extern char DustShaft_dtor[];
extern char Effect6FF60_Draw[];
extern char Effect6FF60_SetParams[];
extern char Effect6FF60_Start[];
extern char Effect6FF60_Update[];
extern char Effect6FF60_dtor[];
extern char Effect71000_Draw[];
extern char Effect71000_SetParams[];
extern char Effect71000_Start[];
extern char Effect71000_Update[];
extern char Effect71000_dtor[];
extern char Effect726E0_Draw[];
extern char Effect726E0_SetParams[];
extern char Effect726E0_Start[];
extern char Effect726E0_Update[];
extern char Effect726E0_dtor[];
extern char Effect737D0_Draw[];
extern char Effect737D0_SetParams[];
extern char Effect737D0_Start[];
extern char Effect737D0_Update[];
extern char Effect737D0_dtor[];
extern char Effect78BC0_Draw[];
extern char Effect78BC0_SetParams[];
extern char Effect78BC0_Start[];
extern char Effect78BC0_Update[];
extern char Effect78BC0_dtor[];
extern char Effect79B00_Draw[];
extern char Effect79B00_SetParams[];
extern char Effect79B00_Start[];
extern char Effect79B00_Update[];
extern char Effect79B00_dtor[];
extern char Effect79FF0_Draw[];
extern char Effect79FF0_SetParams[];
extern char Effect79FF0_Start[];
extern char Effect79FF0_Update[];
extern char Effect79FF0_dtor[];
extern char Effect7A3D0_Draw[];
extern char Effect7A3D0_Start[];
extern char Effect7A3D0_Update[];
extern char Effect7A3D0_dtor[];
extern char EffectBase_Query[];
extern char EffectBase_SetParams[];
extern char EffectBase_dtor[];
extern char Embers_Draw[];
extern char Embers_SetParams[];
extern char Embers_Start[];
extern char Embers_Update[];
extern char Embers_dtor[];
extern char EvEffect7F_Draw[];
extern char EvEffect7F_SetParams[];
extern char EvEffect7F_Start[];
extern char EvEffect7F_Update[];
extern char EvEffect7F_dtor[];
extern char EvEffect86_Draw[];
extern char EvEffect86_SetParams[];
extern char EvEffect86_Start[];
extern char EvEffect86_Update[];
extern char EvEffect86_dtor[];
extern char EventCam_Set[];
extern char EventCmd_Skip[];
extern char EventCond_Eval[];
extern char EventCond_Skip[];
extern char EventHumanModel_BufferSize[];
extern char EventHumanModel_Frame[];
extern char EventHumanModel_Loaded[];
extern char EventHumanModel_ModelFile[];
extern char EventHumanModel_MotionTable[];
extern char EventHumanModel_Part0[];
extern char EventHumanModel_Part1[];
extern char EventHumanModel_Part2[];
extern char EventHumanModel_Part3[];
extern char EventHumanModel_Part4[];
extern char EventHumanModel_SetFlag[];
extern char EventHumanModel_SetPoint[];
extern char EventHumanModel_ShowParts[];
extern char EventHumanModel_Springs[];
extern char EventHumanModel_Textures[];
extern char EventHumanModel_Textures2[];
extern char EventHumanModel_dtor[];
extern char EventPoint_dtor[];
extern char EventsBase_dtor[];
extern char Events_AreaMiddle[];
extern char Events_CharEnter[];
extern char Events_CharInArea[];
extern char Events_ClearBit[];
extern char Events_CloseMessage[];
extern char Events_CueMoved[];
extern char Events_CurrentBlock[];
extern char Events_DrawFade[];
extern char Events_Fade[];
extern char Events_Get703[];
extern char Events_GetVar[];
extern char Events_GiveScript[];
extern char Events_InArea[];
extern char Events_NearestPoint[];
extern char Events_OpenMessage[];
extern char Events_RoomPoint[];
extern char Events_ScenePlaying[];
extern char Events_ScriptCharValue[];
extern char Events_ScriptRoom[];
extern char Events_SetBit[];
extern char Events_SetFadeColour[];
extern char Events_SetPoint[];
extern char Events_SetScript[];
extern char Events_SetVar[];
extern char Events_ShowText[];
extern char Events_StartAction[];
extern char Events_TestBit[];
extern char Events_ThingPlace[];
extern char Events_dtor[];
extern char Exception_What[];
extern char FallingDrops_Draw[];
extern char FallingDrops_Start[];
extern char FallingDrops_Update[];
extern char FallingDrops_dtor[];
extern char FionaModel_BufferSize[];
extern char FionaModel_Frame[];
extern char FionaModel_Loaded[];
extern char FionaModel_MarkerFile[];
extern char FionaModel_ModelFile[];
extern char FionaModel_MotionTable[];
extern char FionaModel_SetVector[];
extern char FionaModel_Springs[];
extern char FionaModel_Textures[];
extern char FionaModel_Textures2[];
extern char FionaModel_dtor[];
extern char Fiona_Activate[];
extern char Fiona_AnimUpdate[];
extern char Fiona_BackToIdle[];
extern char Fiona_CanInteract[];
extern char Fiona_Cleanup[];
extern char Fiona_Deactivate[];
extern char Fiona_Disable[];
extern char Fiona_FilesLoaded[];
extern char Fiona_FollowAnim[];
extern char Fiona_ForgetPath[];
extern char Fiona_FullStop[];
extern char Fiona_Halt[];
extern char Fiona_Interrupted[];
extern char Fiona_IsBusy[];
extern char Fiona_LightChange[];
extern char Fiona_LoadFiles[];
extern char Fiona_LoadMessage[];
extern char Fiona_PlaceInRoom[];
extern char Fiona_PlaceOn[];
extern char Fiona_PointOfInterest[];
extern char Fiona_RememberPos[];
extern char Fiona_Requests[];
extern char Fiona_Reset[];
extern char Fiona_StateActionOver[];
extern char Fiona_StateAfterCaught[];
extern char Fiona_StateAfterFall[];
extern char Fiona_StateAfterKnockDown[];
extern char Fiona_StateAfterThrow[];
extern char Fiona_StateBlock[];
extern char Fiona_StateCaught[];
extern char Fiona_StateCaughtCrawling[];
extern char Fiona_StateCmdAnimDone[];
extern char Fiona_StateCmdDone[];
extern char Fiona_StateCmdPath[];
extern char Fiona_StateCmdPathSpot[];
extern char Fiona_StateCmdPathTo[];
extern char Fiona_StateCmdPathTurn[];
extern char Fiona_StateCmdToSpot[];
extern char Fiona_StateCmdTurn[];
extern char Fiona_StateCmdWalkAnim[];
extern char Fiona_StateCmdWalkSpot[];
extern char Fiona_StateCrawlDown[];
extern char Fiona_StateCrawlDrop[];
extern char Fiona_StateCrawlFall[];
extern char Fiona_StateCrawlPull[];
extern char Fiona_StateDoorAnim[];
extern char Fiona_StateDoorStart[];
extern char Fiona_StateDoorStepOut[];
extern char Fiona_StateDoorWalk[];
extern char Fiona_StateDragged[];
extern char Fiona_StateDraggedOff[];
extern char Fiona_StateFall13[];
extern char Fiona_StateFallen[];
extern char Fiona_StateFleeStart[];
extern char Fiona_StateFleeTurn[];
extern char Fiona_StateGesture[];
extern char Fiona_StateGestureEnd[];
extern char Fiona_StateGestureLeadIn[];
extern char Fiona_StateGetUp[];
extern char Fiona_StateGrabbed[];
extern char Fiona_StateHandSpot[];
extern char Fiona_StateHeld[];
extern char Fiona_StateHeldCommand[];
extern char Fiona_StateHeldGesture[];
extern char Fiona_StateHeldStart[];
extern char Fiona_StateHeldStopped[];
extern char Fiona_StateHeldWalk[];
extern char Fiona_StateHitByDoor[];
extern char Fiona_StateHold[];
extern char Fiona_StateIdleAtEvent[];
extern char Fiona_StateIdleAtEvent2[];
extern char Fiona_StateIdleMove[];
extern char Fiona_StateIdleStep[];
extern char Fiona_StateKick[];
extern char Fiona_StateKickStart[];
extern char Fiona_StateKnockedDown[];
extern char Fiona_StateLadder[];
extern char Fiona_StateLadderOff[];
extern char Fiona_StateLadderWalk[];
extern char Fiona_StateLedAway[];
extern char Fiona_StateLedByHand[];
extern char Fiona_StateLedHand[];
extern char Fiona_StateLedSpot[];
extern char Fiona_StateLedWalking[];
extern char Fiona_StateLetGo[];
extern char Fiona_StateLetGoStart[];
extern char Fiona_StateLockedQuick[];
extern char Fiona_StateLockedSlow[];
extern char Fiona_StateLockedSpot[];
extern char Fiona_StateLockedTry[];
extern char Fiona_StateLookAround[];
extern char Fiona_StateLookEnd[];
extern char Fiona_StateOnGround[];
extern char Fiona_StateOnLadder[];
extern char Fiona_StatePanicAttack[];
extern char Fiona_StatePanicFall[];
extern char Fiona_StatePulledFree[];
extern char Fiona_StatePush[];
extern char Fiona_StatePushLetGo[];
extern char Fiona_StatePushStep[];
extern char Fiona_StateRequestTurn[];
extern char Fiona_StateScriptDoor[];
extern char Fiona_StateScriptDoorOpened[];
extern char Fiona_StateScriptDoorWalk[];
extern char Fiona_StateShove[];
extern char Fiona_StateShoveStart[];
extern char Fiona_StateSpecialEntry[];
extern char Fiona_StateStep[];
extern char Fiona_StateStrike[];
extern char Fiona_StateStruggle[];
extern char Fiona_StateThrown[];
extern char Fiona_StateTurnOnSpot[];
extern char Fiona_StateTurnStanding[];
extern char Fiona_StateTurnStick[];
extern char Fiona_StateTurnUntilDone[];
extern char Fiona_StateWaitHewie[];
extern char Fiona_StateWalkThenGesture[];
extern char Fiona_Think[];
extern char Fiona_Unload[];
extern char Fiona_Update[];
extern char Fiona_Vt34[];
extern char Fiona_Vt38[];
extern char Fiona_dtor[];
extern char Fire_Draw[];
extern char Fire_SetParams[];
extern char Fire_Start[];
extern char Fire_Update[];
extern char Fire_dtor[];
extern char FixModelDraw_dtor[];
extern char FixModel_Draw[];
extern char FloorGlow_Draw[];
extern char FloorGlow_SetParams[];
extern char FloorGlow_Start[];
extern char FloorGlow_Update[];
extern char FloorGlow_dtor[];
extern char FloorSplat_Draw[];
extern char FloorSplat_SetParams[];
extern char FloorSplat_Start[];
extern char FloorSplat_Update[];
extern char FloorSplat_dtor[];
extern char FogDrawer_dtor[];
extern char Fog_Draw[];
extern char Fog_SetParams[];
extern char Fog_Start[];
extern char Fog_Update[];
extern char Fog_dtor[];
extern char Gallery_StateArtList[];
extern char Gallery_StateArtView[];
extern char Gallery_StateModel[];
extern char Gallery_StateModelChosen[];
extern char Gallery_StateModelLoad[];
extern char Gallery_StateMusic[];
extern char Gallery_StatePicture[];
extern char GameBase_dtor[];
extern char GameOverBase_dtor[];
extern char GameOver_StateDone[];
extern char GameOver_StateMode3[];
extern char GameOver_StateMovie[];
extern char GameOver_StateOthers[];
extern char GameOver_StateSpecial[];
extern char GameOver_StateStart[];
extern char Game_Init[];
extern char Game_Resident102C0[];
extern char Game_Resident1AC0[];
extern char Game_Resident2AC0[];
extern char Game_Resident312C0[];
extern char Game_Resident38AC0[];
extern char Game_ResidentB2C0[];
extern char Game_StartNextScene[];
extern char Game_StateMain[];
extern char Game_StateShutdown[];
extern char Game_dtor[];
extern char Gesture_Back[];
extern char Gesture_Forward[];
extern char Gesture_Left[];
extern char Gesture_R3[];
extern char Gesture_Right[];
extern char Glint_Draw[];
extern char Glint_Start[];
extern char Glint_Update[];
extern char Glint_dtor[];
extern char HairPoint2_AdjustBone[];
extern char HairPoint2_Frame[];
extern char HairPoint_Pose[];
extern char HairPoint_Step[];
extern char HangPoint_Frame[];
extern char HangingPart_Step[];
extern char Heap_Alloc[];
extern char Heap_Free[];
extern char Heap_Init[];
extern char Heap_dtor[];
extern char Helper469D00_dtor[];
extern char Helper46FC30_dtor[];
extern char Hewie_Activate[];
extern char Hewie_AnimUpdate[];
extern char Hewie_BackToIdle[];
extern char Hewie_CanInteract[];
extern char Hewie_Cleanup[];
extern char Hewie_Deactivate[];
extern char Hewie_Disable[];
extern char Hewie_FilesLoaded[];
extern char Hewie_FollowAnim[];
extern char Hewie_FullStop[];
extern char Hewie_Halt[];
extern char Hewie_Interrupted[];
extern char Hewie_IsBusy[];
extern char Hewie_LeftBehind[];
extern char Hewie_LightChange[];
extern char Hewie_LoadFiles[];
extern char Hewie_LoadState[];
extern char Hewie_PlaceInRoom[];
extern char Hewie_PlaceOn[];
extern char Hewie_PointOfInterest[];
extern char Hewie_RememberPos[];
extern char Hewie_Requests[];
extern char Hewie_Reset[];
extern char Hewie_SaveState[];
extern char Hewie_State1740[];
extern char Hewie_State1770[];
extern char Hewie_State18B0[];
extern char Hewie_State1920[];
extern char Hewie_State1980[];
extern char Hewie_State1990[];
extern char Hewie_State19A0[];
extern char Hewie_State19B0[];
extern char Hewie_State1A60[];
extern char Hewie_State1AA0[];
extern char Hewie_State1AF0[];
extern char Hewie_State1B00[];
extern char Hewie_State1B40[];
extern char Hewie_State1B60[];
extern char Hewie_State1C48[];
extern char Hewie_State1C58[];
extern char Hewie_State1C98[];
extern char Hewie_State1CC8[];
extern char Hewie_State1D18[];
extern char Hewie_State1D38[];
extern char Hewie_State1D88[];
extern char Hewie_State1D98[];
extern char Hewie_State1DC8[];
extern char Hewie_State1E48[];
extern char Hewie_State1F28[];
extern char Hewie_State1F88[];
extern char Hewie_State2018[];
extern char Hewie_State2098[];
extern char Hewie_State20A8[];
extern char Hewie_State20C8[];
extern char Hewie_State2138[];
extern char Hewie_State2168[];
extern char Hewie_State2198[];
extern char Hewie_State22B8[];
extern char Hewie_State22E8[];
extern char Hewie_State2318[];
extern char Hewie_State2358[];
extern char Hewie_State2388[];
extern char Hewie_State23D8[];
extern char Hewie_State2418[];
extern char Hewie_StateAfter27[];
extern char Hewie_StateAfter29[];
extern char Hewie_StateAfter2B[];
extern char Hewie_StateAnimNext[];
extern char Hewie_StateAnimOver[];
extern char Hewie_StateAnimOver2[];
extern char Hewie_StateAnimOver3[];
extern char Hewie_StateBackOnMesh[];
extern char Hewie_StateBackToNormal[];
extern char Hewie_StateBark[];
extern char Hewie_StateBarkAtFiona[];
extern char Hewie_StateBarkAtTarget[];
extern char Hewie_StateBehindFiona[];
extern char Hewie_StateBiteHold[];
extern char Hewie_StateBlock[];
extern char Hewie_StateByFiona[];
extern char Hewie_StateCalm[];
extern char Hewie_StateClamped[];
extern char Hewie_StateCloseIn[];
extern char Hewie_StateCloseOnTarget[];
extern char Hewie_StateComeToCommand[];
extern char Hewie_StateComeToFiona[];
extern char Hewie_StateFaceScent[];
extern char Hewie_StateFaceTarget[];
extern char Hewie_StateFalling[];
extern char Hewie_StateFetch[];
extern char Hewie_StateFlank[];
extern char Hewie_StateFlee[];
extern char Hewie_StateFollowCommand[];
extern char Hewie_StateFollowMover[];
extern char Hewie_StateGoForPursuer[];
extern char Hewie_StateGoForPursuer2[];
extern char Hewie_StateGoToExit[];
extern char Hewie_StateGoesForPursuer[];
extern char Hewie_StateGuard[];
extern char Hewie_StateHanging[];
extern char Hewie_StateHeadForSpot[];
extern char Hewie_StateJumpArc[];
extern char Hewie_StateKeepAway[];
extern char Hewie_StateKeepBehind[];
extern char Hewie_StateKeepNear[];
extern char Hewie_StateKnockedDown[];
extern char Hewie_StateLanding[];
extern char Hewie_StateLeap[];
extern char Hewie_StateLeapAtPursuer[];
extern char Hewie_StateLeapBite[];
extern char Hewie_StateLeapGrip[];
extern char Hewie_StateLeapLands[];
extern char Hewie_StateLeapLaunch[];
extern char Hewie_StateLeapTakeOff[];
extern char Hewie_StateLookLow[];
extern char Hewie_StateLoudNoise[];
extern char Hewie_StateMoveAlong[];
extern char Hewie_StateOffMesh[];
extern char Hewie_StateOnEvent20[];
extern char Hewie_StatePending[];
extern char Hewie_StatePlanPursuer[];
extern char Hewie_StatePlayAnim[];
extern char Hewie_StatePlayToEnd[];
extern char Hewie_StatePose2[];
extern char Hewie_StatePose4[];
extern char Hewie_StatePraised[];
extern char Hewie_StatePursuerNear[];
extern char Hewie_StateRoam[];
extern char Hewie_StateRootMotion[];
extern char Hewie_StateRun[];
extern char Hewie_StateRunAtPursuer[];
extern char Hewie_StateRunForSpot[];
extern char Hewie_StateRunSpot[];
extern char Hewie_StateRunToFiona[];
extern char Hewie_StateRunUpJump[];
extern char Hewie_StateRush[];
extern char Hewie_StateScramble[];
extern char Hewie_StateSetOff[];
extern char Hewie_StateSlideToCommand[];
extern char Hewie_StateSlideToFiona[];
extern char Hewie_StateSlope[];
extern char Hewie_StateSqueeze[];
extern char Hewie_StateSteer[];
extern char Hewie_StateSteered[];
extern char Hewie_StateStepAwayFiona[];
extern char Hewie_StateStepAwayTarget[];
extern char Hewie_StateTackle[];
extern char Hewie_StateTargetPursuer[];
extern char Hewie_StateTense[];
extern char Hewie_StateToDefault[];
extern char Hewie_StateToDoor[];
extern char Hewie_StateTricks[];
extern char Hewie_StateTurnStart[];
extern char Hewie_StateTurnTo[];
extern char Hewie_StateTurnWithFiona[];
extern char Hewie_StateTurning[];
extern char Hewie_StateWaitForFiona[];
extern char Hewie_StateWalkHeading[];
extern char Hewie_StateWalkOut[];
extern char Hewie_StateWalkPath[];
extern char Hewie_StateWalkPath2[];
extern char Hewie_StateWander[];
extern char Hewie_StateWary[];
extern char Hewie_StateWhine[];
extern char Hewie_TakeDamage[];
extern char Hewie_Think[];
extern char Hewie_ToIdle[];
extern char Hewie_Unload[];
extern char Hewie_Update[];
extern char Hewie_Vt34[];
extern char Hewie_Vt38[];
extern char Hewie_dtor[];
extern char HumanModel_AdjustBone[];
extern char HumanModel_BodyFrames[];
extern char HumanModel_FeetUp[];
extern char HumanModel_Frame[];
extern char HumanModel_HeadBone[];
extern char HumanModel_HeadPos[];
extern char HumanModel_Loaded[];
extern char HumanModel_PlantFeet[];
extern char HumanModel_Vt3C[];
extern char HumanModel_Vt58[];
extern char HumanModel_Vt74[];
extern char HumanModel_Vt78[];
extern char HumanModel_Vt7C[];
extern char HumanModel_dtor[];
extern char IK2_Loaded[];
extern char IK2_dtor[];
extern char IK3_Loaded[];
extern char IK3_dtor[];
extern char Item00_LoadPicture[];
extern char Item00_Use[];
extern char Item00_dtor[];
extern char Item01_CountUp[];
extern char Item01_IsCounted[];
extern char Item01_LoadPicture[];
extern char Item01_Use[];
extern char Item01_dtor[];
extern char Item02_LoadPicture[];
extern char Item02_Use[];
extern char Item02_dtor[];
extern char Item03_LoadPicture[];
extern char Item03_Use[];
extern char Item03_dtor[];
extern char Item04_LoadPicture[];
extern char Item04_Use[];
extern char Item04_dtor[];
extern char Item05_LoadPicture[];
extern char Item05_Use[];
extern char Item05_dtor[];
extern char Item06_LoadPicture[];
extern char Item06_Use[];
extern char Item06_dtor[];
extern char Item07_LoadPicture[];
extern char Item07_Use[];
extern char Item07_dtor[];
extern char Item08_LoadPicture[];
extern char Item08_Use[];
extern char Item08_dtor[];
extern char Item09_LoadPicture[];
extern char Item09_Use[];
extern char Item09_dtor[];
extern char Item0A_LoadPicture[];
extern char Item0A_Use[];
extern char Item0A_dtor[];
extern char Item0B_LoadPicture[];
extern char Item0B_Use[];
extern char Item0B_dtor[];
extern char Item0C_LoadPicture[];
extern char Item0C_Use[];
extern char Item0C_dtor[];
extern char Item0D_LoadPicture[];
extern char Item0D_Use[];
extern char Item0D_dtor[];
extern char Item0E_LoadPicture[];
extern char Item0E_Use[];
extern char Item0E_dtor[];
extern char Item0F_LoadPicture[];
extern char Item0F_Use[];
extern char Item0F_dtor[];
extern char Item10_LoadPicture[];
extern char Item10_Use[];
extern char Item10_dtor[];
extern char Item11_LoadPicture[];
extern char Item11_Use[];
extern char Item11_dtor[];
extern char Item12_LoadPicture[];
extern char Item12_Use[];
extern char Item12_dtor[];
extern char Item13_LoadPicture[];
extern char Item13_Use[];
extern char Item13_dtor[];
extern char Item14_LoadPicture[];
extern char Item14_Use[];
extern char Item14_dtor[];
extern char Item15_LoadPicture[];
extern char Item15_Use[];
extern char Item15_dtor[];
extern char Item16_LoadPicture[];
extern char Item16_Use[];
extern char Item16_dtor[];
extern char Item17_LoadPicture[];
extern char Item17_Use[];
extern char Item17_dtor[];
extern char Item18_Actions[];
extern char Item18_LoadPicture[];
extern char Item18_Use[];
extern char Item18_dtor[];
extern char Item19_Actions[];
extern char Item19_LoadPicture[];
extern char Item19_Use[];
extern char Item19_dtor[];
extern char Item1A_Actions[];
extern char Item1A_LoadPicture[];
extern char Item1A_Use[];
extern char Item1A_dtor[];
extern char Item1B_Actions[];
extern char Item1B_LoadPicture[];
extern char Item1B_Use[];
extern char Item1B_dtor[];
extern char Item1C_Actions[];
extern char Item1C_LoadPicture[];
extern char Item1C_Use[];
extern char Item1C_dtor[];
extern char Item1D_Actions[];
extern char Item1D_LoadPicture[];
extern char Item1D_Use[];
extern char Item1D_dtor[];
extern char Item1E_LoadPicture[];
extern char Item1E_Use[];
extern char Item1E_dtor[];
extern char Item1F_LoadPicture[];
extern char Item1F_Use[];
extern char Item1F_dtor[];
extern char Item20_LoadPicture[];
extern char Item20_Use[];
extern char Item20_dtor[];
extern char Item21_LoadPicture[];
extern char Item21_Use[];
extern char Item21_dtor[];
extern char Item22_LoadPicture[];
extern char Item22_Use[];
extern char Item22_dtor[];
extern char Item23_LoadPicture[];
extern char Item23_Use[];
extern char Item23_dtor[];
extern char Item24_LoadPicture[];
extern char Item24_Use[];
extern char Item24_dtor[];
extern char Item25_LoadPicture[];
extern char Item25_Use[];
extern char Item25_dtor[];
extern char Item26_LoadPicture[];
extern char Item26_Use[];
extern char Item26_dtor[];
extern char Item27_LoadPicture[];
extern char Item27_Use[];
extern char Item27_dtor[];
extern char Item28_LoadPicture[];
extern char Item28_Use[];
extern char Item28_dtor[];
extern char Item29_LoadPicture[];
extern char Item29_Use[];
extern char Item29_dtor[];
extern char Item3E_Use[];
extern char Item3E_dtor[];
extern char Item40_LoadPicture[];
extern char Item40_SortKey[];
extern char Item40_Use[];
extern char Item40_dtor[];
extern char Item41_LoadPicture[];
extern char Item41_SortKey[];
extern char Item41_Use[];
extern char Item41_dtor[];
extern char Item42_Actions[];
extern char Item42_LoadPicture[];
extern char Item42_SortKey[];
extern char Item42_Use[];
extern char Item42_dtor[];
extern char Item43_Actions[];
extern char Item43_LoadPicture[];
extern char Item43_SortKey[];
extern char Item43_Use[];
extern char Item43_dtor[];
extern char Item44_Actions[];
extern char Item44_LoadPicture[];
extern char Item44_SortKey[];
extern char Item44_Use[];
extern char Item44_dtor[];
extern char Item45_Actions[];
extern char Item45_LoadPicture[];
extern char Item45_SortKey[];
extern char Item45_Use[];
extern char Item45_dtor[];
extern char Item46_Actions[];
extern char Item46_LoadPicture[];
extern char Item46_SortKey[];
extern char Item46_Use[];
extern char Item46_dtor[];
extern char Item47_Actions[];
extern char Item47_LoadPicture[];
extern char Item47_SortKey[];
extern char Item47_Use[];
extern char Item47_dtor[];
extern char Item48_Actions[];
extern char Item48_LoadPicture[];
extern char Item48_SortKey[];
extern char Item48_Use[];
extern char Item48_dtor[];
extern char Item49_Actions[];
extern char Item49_LoadPicture[];
extern char Item49_SortKey[];
extern char Item49_Use[];
extern char Item49_dtor[];
extern char Item4A_Actions[];
extern char Item4A_LoadPicture[];
extern char Item4A_SortKey[];
extern char Item4A_Use[];
extern char Item4A_dtor[];
extern char Item4B_Actions[];
extern char Item4B_LoadPicture[];
extern char Item4B_SortKey[];
extern char Item4B_Use[];
extern char Item4B_dtor[];
extern char Item4C_Actions[];
extern char Item4C_LoadPicture[];
extern char Item4C_SortKey[];
extern char Item4C_Use[];
extern char Item4C_dtor[];
extern char Item60_LoadPicture[];
extern char Item60_SortKey[];
extern char Item60_Use[];
extern char Item60_dtor[];
extern char Item61_LoadPicture[];
extern char Item61_SortKey[];
extern char Item61_Use[];
extern char Item61_dtor[];
extern char Item62_LoadPicture[];
extern char Item62_SortKey[];
extern char Item62_Use[];
extern char Item62_dtor[];
extern char Item63_Actions[];
extern char Item63_LoadPicture[];
extern char Item63_SortKey[];
extern char Item63_Use[];
extern char Item63_dtor[];
extern char Item64_Actions[];
extern char Item64_LoadPicture[];
extern char Item64_SortKey[];
extern char Item64_Use[];
extern char Item64_dtor[];
extern char Item65_Actions[];
extern char Item65_LoadPicture[];
extern char Item65_SortKey[];
extern char Item65_Use[];
extern char Item65_dtor[];
extern char Item66_Actions[];
extern char Item66_LoadPicture[];
extern char Item66_SortKey[];
extern char Item66_Use[];
extern char Item66_dtor[];
extern char Item70_LoadPicture[];
extern char Item70_SortKey[];
extern char Item70_dtor[];
extern char Item71_LoadPicture[];
extern char Item71_SortKey[];
extern char Item71_dtor[];
extern char Item72_LoadPicture[];
extern char Item72_SortKey[];
extern char Item72_dtor[];
extern char Item73_LoadPicture[];
extern char Item73_SortKey[];
extern char Item73_dtor[];
extern char Item74_LoadPicture[];
extern char Item74_SortKey[];
extern char Item74_dtor[];
extern char Item75_LoadPicture[];
extern char Item75_SortKey[];
extern char Item75_dtor[];
extern char Item80_LoadPicture[];
extern char Item80_SortKey[];
extern char Item80_dtor[];
extern char Item81_LoadPicture[];
extern char Item81_SortKey[];
extern char Item81_dtor[];
extern char Item82_LoadPicture[];
extern char Item82_SortKey[];
extern char Item82_dtor[];
extern char Item83_LoadPicture[];
extern char Item83_SortKey[];
extern char Item83_dtor[];
extern char Item86_LoadPicture[];
extern char Item86_SortKey[];
extern char Item86_dtor[];
extern char Item87_LoadPicture[];
extern char Item87_SortKey[];
extern char Item87_dtor[];
extern char Item88_LoadPicture[];
extern char Item88_SortKey[];
extern char Item88_dtor[];
extern char Item89_LoadPicture[];
extern char Item89_SortKey[];
extern char Item89_dtor[];
extern char Item8A_LoadPicture[];
extern char Item8A_SortKey[];
extern char Item8A_dtor[];
extern char Item8B_LoadPicture[];
extern char Item8B_SortKey[];
extern char Item8B_dtor[];
extern char Item8C_LoadPicture[];
extern char Item8C_SortKey[];
extern char Item8C_dtor[];
extern char Item8D_LoadPicture[];
extern char Item8D_SortKey[];
extern char Item8D_dtor[];
extern char Item90_LoadPicture[];
extern char Item90_SortKey[];
extern char Item90_dtor[];
extern char Item91_LoadPicture[];
extern char Item91_SortKey[];
extern char Item91_dtor[];
extern char Item92_LoadPicture[];
extern char Item92_SortKey[];
extern char Item92_dtor[];
extern char Item93_LoadPicture[];
extern char Item93_SortKey[];
extern char Item93_dtor[];
extern char Item94_LoadPicture[];
extern char Item94_SortKey[];
extern char Item94_dtor[];
extern char Item95_LoadPicture[];
extern char Item95_SortKey[];
extern char Item95_dtor[];
extern char Item97_LoadPicture[];
extern char Item97_SortKey[];
extern char Item97_dtor[];
extern char Item98_LoadPicture[];
extern char Item98_SortKey[];
extern char Item98_dtor[];
extern char Item9B_LoadPicture[];
extern char Item9B_SortKey[];
extern char Item9B_dtor[];
extern char ItemA0_LoadPicture[];
extern char ItemA0_Use[];
extern char ItemA0_dtor[];
extern char ItemA1_LoadPicture[];
extern char ItemA1_Use[];
extern char ItemA1_dtor[];
extern char ItemA2_LoadPicture[];
extern char ItemA2_Use[];
extern char ItemA2_dtor[];
extern char ItemA3_LoadPicture[];
extern char ItemA3_Use[];
extern char ItemA3_dtor[];
extern char ItemA4_LoadPicture[];
extern char ItemA4_Use[];
extern char ItemA4_dtor[];
extern char ItemA5_LoadPicture[];
extern char ItemA5_Use[];
extern char ItemA5_dtor[];
extern char ItemA6_LoadPicture[];
extern char ItemA6_Use[];
extern char ItemA6_dtor[];
extern char ItemA7_LoadPicture[];
extern char ItemA7_Use[];
extern char ItemA7_dtor[];
extern char ItemA8_LoadPicture[];
extern char ItemA8_Use[];
extern char ItemA8_dtor[];
extern char ItemA9_LoadPicture[];
extern char ItemA9_Use[];
extern char ItemA9_dtor[];
extern char ItemAA_LoadPicture[];
extern char ItemAA_Use[];
extern char ItemAA_dtor[];
extern char ItemAB_LoadPicture[];
extern char ItemAB_Use[];
extern char ItemAB_dtor[];
extern char ItemAC_LoadPicture[];
extern char ItemAC_Use[];
extern char ItemAC_dtor[];
extern char ItemClass6B00_Actions[];
extern char ItemClass6B00_IsCounted[];
extern char ItemClass6B00_Kind[];
extern char ItemClass6B00_PartnerCheck[];
extern char ItemClass6B00_dtor[];
extern char ItemClassC920_Actions[];
extern char ItemClassC920_IsCounted[];
extern char ItemClassC920_Kind[];
extern char ItemClassC920_dtor[];
extern char ItemClassCB50_Actions[];
extern char ItemClassCB50_IsCounted[];
extern char ItemClassCB50_Kind[];
extern char ItemClassCB50_Use[];
extern char ItemClassCB50_dtor[];
extern char ItemClassCCE0_Actions[];
extern char ItemClassCCE0_IsCounted[];
extern char ItemClassCCE0_Kind[];
extern char ItemClassCCE0_dtor[];
extern char ItemClassCE70_Actions[];
extern char ItemClassCE70_IsCounted[];
extern char ItemClassCE70_Kind[];
extern char ItemClassCE70_dtor[];
extern char ItemClassD000_Actions[];
extern char ItemClassD000_IsCounted[];
extern char ItemClassD000_Kind[];
extern char ItemClassD000_dtor[];
extern char ItemClassD320_Actions[];
extern char ItemClassD320_IsCounted[];
extern char ItemClassD320_Kind[];
extern char ItemClassD320_dtor[];
extern char ItemClassD640_Actions[];
extern char ItemClassD640_IsCounted[];
extern char ItemClassD640_Kind[];
extern char ItemClassD640_dtor[];
extern char ItemClassF430_Actions[];
extern char ItemClassF430_Get20[];
extern char ItemClassF430_IsCounted[];
extern char ItemClassF430_Kind[];
extern char ItemClassF430_LoadPicture[];
extern char ItemClassF430_Use[];
extern char ItemClassF430_dtor[];
extern char ItemFound_StateShow[];
extern char Kind08_EventState[];
extern char Kind08_GrabOrder[];
extern char Kind08_Setup[];
extern char Kind08_ShowUp[];
extern char Kind08_Table6C[];
extern char Kind08_Table70[];
extern char Kind08_dtor[];
extern char Kind09Model_Loaded[];
extern char Kind09Model_Part0[];
extern char Kind09Model_Part1[];
extern char Kind09Model_Part2[];
extern char Kind09Model_Part3[];
extern char Kind09Model_SecondaryMotion[];
extern char Kind09Model_Vt3C[];
extern char Kind09Model_dtor[];
extern char Kind09_Behaviour25C[];
extern char Kind09_BehaviourIdle[];
extern char Kind09_BehaviourSearch[];
extern char Kind09_EventState[];
extern char Kind09_GrabOrder[];
extern char Kind09_ModelFiles[];
extern char Kind09_MotionFiles[];
extern char Kind09_OffscreenStep[];
extern char Kind09_ShowUp[];
extern char Kind09_Update[];
extern char Kind09_dtor[];
extern char Kind12Model_Loaded[];
extern char Kind12Model_Part0[];
extern char Kind12Model_Part1[];
extern char Kind12Model_Part2[];
extern char Kind12Model_Part3[];
extern char Kind12Model_SecondaryMotion[];
extern char Kind12Model_dtor[];
extern char Kind12_AttackRange[];
extern char Kind12_AttackTable[];
extern char Kind12_Deactivate[];
extern char Kind12_EnterRoom[];
extern char Kind12_EventState[];
extern char Kind12_ExitDone[];
extern char Kind12_GrabOrder[];
extern char Kind12_ModelFiles[];
extern char Kind12_MotionFiles[];
extern char Kind12_ReachHewie[];
extern char Kind12_Setup[];
extern char Kind12_ShowUp[];
extern char Kind12_StopSounds[];
extern char Kind12_Update[];
extern char Kind12_dtor[];
extern char Kind13_EventState[];
extern char Kind13_GrabOrder[];
extern char Kind13_ModelFiles[];
extern char Kind13_MotionFiles[];
extern char Kind13_ShowUp[];
extern char Kind13_dtor[];
extern char Kind14Model_Draw[];
extern char Kind14Model_Frame[];
extern char Kind14Model_Loaded[];
extern char Kind14Model_SecondaryMotion[];
extern char Kind14Model_dtor[];
extern char Kind14_EventState[];
extern char Kind14_GrabOrder[];
extern char Kind14_ModelFiles[];
extern char Kind14_MotionFiles[];
extern char Kind14_Setup[];
extern char Kind14_ShowUp[];
extern char Kind14_dtor[];
extern char Kind15_dtor[];
extern char Kind16_EventState[];
extern char Kind16_GrabOrder[];
extern char Kind16_ModelFiles[];
extern char Kind16_MotionFiles[];
extern char Kind16_ShowUp[];
extern char Kind16_dtor[];
extern char Kind17_EventState[];
extern char Kind17_GrabOrder[];
extern char Kind17_ModelFiles[];
extern char Kind17_MotionFiles[];
extern char Kind17_ShowUp[];
extern char Kind17_dtor[];
extern char Kind18Model_Frame[];
extern char Kind18Model_Loaded[];
extern char Kind18Model_ShowParts[];
extern char Kind18Model_dtor[];
extern char Kind18_EventState[];
extern char Kind18_GrabOrder[];
extern char Kind18_ModelFiles[];
extern char Kind18_MotionFiles[];
extern char Kind18_ShowUp[];
extern char Kind18_dtor[];
extern char Kind19_EventState[];
extern char Kind19_GrabOrder[];
extern char Kind19_ModelFiles[];
extern char Kind19_MotionFiles[];
extern char Kind19_ShowUp[];
extern char Kind19_dtor[];
extern char Kind20_EventState[];
extern char Kind20_GrabOrder[];
extern char Kind20_ModelFiles[];
extern char Kind20_MotionFiles[];
extern char Kind20_ShowUp[];
extern char Kind20_dtor[];
extern char Kind21_EventState[];
extern char Kind21_GrabOrder[];
extern char Kind21_ModelFiles[];
extern char Kind21_MotionFiles[];
extern char Kind21_ShowUp[];
extern char Kind21_dtor[];
extern char Kind22_dtor[];
extern char Kind23Model_Loaded[];
extern char Kind23Model_Part0[];
extern char Kind23Model_Part1[];
extern char Kind23Model_Part2[];
extern char Kind23Model_Part3[];
extern char Kind23Model_SecondaryMotion[];
extern char Kind23Model_Vt2C[];
extern char Kind23Model_Vt30[];
extern char Kind23Model_Vt3C[];
extern char Kind23Model_Vt98[];
extern char Kind23Model_Vt9C[];
extern char Kind23Model_dtor[];
extern char Kind24_EventState[];
extern char Kind24_GrabOrder[];
extern char Kind24_ModelFiles[];
extern char Kind24_MotionFiles[];
extern char Kind24_ShowUp[];
extern char Kind24_dtor[];
extern char Kind25_EventState[];
extern char Kind25_GrabOrder[];
extern char Kind25_ModelFiles[];
extern char Kind25_MotionFiles[];
extern char Kind25_ShowUp[];
extern char Kind25_dtor[];
extern char Kind26_EventState[];
extern char Kind26_GrabOrder[];
extern char Kind26_ModelFiles[];
extern char Kind26_MotionFiles[];
extern char Kind26_ShowUp[];
extern char Kind26_dtor[];
extern char Kind27_ActionOffsets[];
extern char Kind27_Arrived[];
extern char Kind27_AttackAngle[];
extern char Kind27_AttackAnimA[];
extern char Kind27_AttackAnimB[];
extern char Kind27_AttackRange[];
extern char Kind27_AttackTable[];
extern char Kind27_BlockFlags[];
extern char Kind27_ChaseTarget[];
extern char Kind27_Dist2E8[];
extern char Kind27_DoorOffset[];
extern char Kind27_ExitDone[];
extern char Kind27_FollowPathExit[];
extern char Kind27_GoTo[];
extern char Kind27_HeadFor[];
extern char Kind27_HeadForFiona[];
extern char Kind27_LookFrames[];
extern char Kind27_LookSwing[];
extern char Kind27_ModelFiles[];
extern char Kind27_MotionFiles[];
extern char Kind27_OnToNextExit[];
extern char Kind27_PathNodeSound[];
extern char Kind27_PickDestination[];
extern char Kind27_ReachFiona[];
extern char Kind27_ReachHewie[];
extern char Kind27_SetTimer[];
extern char Kind27_Setup[];
extern char Kind27_SpeedBase[];
extern char Kind27_SpeedTop[];
extern char Kind27_StartGrab[];
extern char Kind27_StartLunge[];
extern char Kind27_StartTurnToFiona[];
extern char Kind27_StateBlow[];
extern char Kind27_StateGrab[];
extern char Kind27_StateLunge[];
extern char Kind27_StateNone[];
extern char Kind27_StateStun[];
extern char Kind27_StateTurnToFiona[];
extern char Kind27_Timer10s[];
extern char Kind27_TurnRate[];
extern char Kind27_TurnRateFast[];
extern char Kind27_Update[];
extern char Kind27_WalkToExit[];
extern char Kind27_dtor[];
extern char Kind28_EventState[];
extern char Kind28_GrabOrder[];
extern char Kind28_ModelFiles[];
extern char Kind28_MotionFiles[];
extern char Kind28_ShowUp[];
extern char Kind28_dtor[];
extern char Kind29_EventState[];
extern char Kind29_GrabOrder[];
extern char Kind29_ModelFiles[];
extern char Kind29_MotionFiles[];
extern char Kind29_ShowUp[];
extern char Kind29_dtor[];
extern char Kind30_EventState[];
extern char Kind30_GrabOrder[];
extern char Kind30_ModelFiles[];
extern char Kind30_MotionFiles[];
extern char Kind30_ShowUp[];
extern char Kind30_dtor[];
extern char Kind31_EventState[];
extern char Kind31_GrabOrder[];
extern char Kind31_ModelFiles[];
extern char Kind31_MotionFiles[];
extern char Kind31_ShowUp[];
extern char Kind31_dtor[];
extern char Kind32_EventState[];
extern char Kind32_GrabOrder[];
extern char Kind32_ModelFiles[];
extern char Kind32_MotionFiles[];
extern char Kind32_ShowUp[];
extern char Kind32_dtor[];
extern char Kind33Model_BodyFrames[];
extern char Kind33Model_Part0[];
extern char Kind33Model_Part1[];
extern char Kind33Model_Part2[];
extern char Kind33Model_Part3[];
extern char Kind33Model_SecondaryMotion[];
extern char Kind33Model_dtor[];
extern char Kind33_EventState[];
extern char Kind33_GrabOrder[];
extern char Kind33_ModelFiles[];
extern char Kind33_MotionFiles[];
extern char Kind33_ShowUp[];
extern char Kind33_dtor[];
extern char Kind34_ActionOffsets[];
extern char Kind34_Activate[];
extern char Kind34_AttackAnimA[];
extern char Kind34_AttackAnimB[];
extern char Kind34_AttackTable[];
extern char Kind34_BonePositions[];
extern char Kind34_DoorBreak[];
extern char Kind34_DoorOffset[];
extern char Kind34_ExitArg[];
extern char Kind34_ExitDone[];
extern char Kind34_FilesLoaded[];
extern char Kind34_FrightSeen[];
extern char Kind34_Kind[];
extern char Kind34_ModelFiles[];
extern char Kind34_MotionFiles[];
extern char Kind34_ReachHewie[];
extern char Kind34_SetRage[];
extern char Kind34_Setup[];
extern char Kind34_ThreatAmount[];
extern char Kind34_Update[];
extern char Kind34_dtor[];
extern char Kind35_ActionOffsets[];
extern char Kind35_AttackAnimA[];
extern char Kind35_AttackAnimB[];
extern char Kind35_AttackTable[];
extern char Kind35_BonePositions[];
extern char Kind35_DoorOffset[];
extern char Kind35_FilesLoaded[];
extern char Kind35_FrightSeen[];
extern char Kind35_Kind[];
extern char Kind35_ModelFiles[];
extern char Kind35_MotionFiles[];
extern char Kind35_ReachHewie[];
extern char Kind35_Setup[];
extern char Kind35_ThreatAmount[];
extern char Kind35_Update[];
extern char Kind35_dtor[];
extern char Kind36_ActionOffsets[];
extern char Kind36_AttackAnimA[];
extern char Kind36_AttackAnimB[];
extern char Kind36_AttackTable[];
extern char Kind36_BonePositions[];
extern char Kind36_DoorOffset[];
extern char Kind36_FilesLoaded[];
extern char Kind36_FrightSeen[];
extern char Kind36_Kind[];
extern char Kind36_ModelFiles[];
extern char Kind36_MotionFiles[];
extern char Kind36_ReachHewie[];
extern char Kind36_Setup[];
extern char Kind36_ThreatAmount[];
extern char Kind36_Update[];
extern char Kind36_dtor[];
extern char Kind37_ActionOffsets[];
extern char Kind37_AttackAnimA[];
extern char Kind37_AttackAnimB[];
extern char Kind37_AttackTable[];
extern char Kind37_BehaviourEnded[];
extern char Kind37_ChaseDecision[];
extern char Kind37_DoorOffset[];
extern char Kind37_ExitDone[];
extern char Kind37_FrightAttack[];
extern char Kind37_FrightSeen[];
extern char Kind37_LightChange[];
extern char Kind37_ModelFiles[];
extern char Kind37_MotionFiles[];
extern char Kind37_ReachHewie[];
extern char Kind37_Setup[];
extern char Kind37_SlowWalkAnim[];
extern char Kind37_StateBehaviour[];
extern char Kind37_StateChase[];
extern char Kind37_Update[];
extern char Kind37_dtor[];
extern char Kind38_EventState[];
extern char Kind38_GrabOrder[];
extern char Kind38_ModelFiles[];
extern char Kind38_MotionFiles[];
extern char Kind38_ShowUp[];
extern char Kind38_dtor[];
extern char Kind39_ActionOffsets[];
extern char Kind39_Activate[];
extern char Kind39_AttackAnimA[];
extern char Kind39_AttackAnimB[];
extern char Kind39_AttackRange[];
extern char Kind39_AttackTable[];
extern char Kind39_BonePositions[];
extern char Kind39_ExitDone[];
extern char Kind39_FrightAttack[];
extern char Kind39_FrightSeen[];
extern char Kind39_ModelFiles[];
extern char Kind39_MotionFiles[];
extern char Kind39_ReachHewie[];
extern char Kind39_Setup[];
extern char Kind39_StartGrab[];
extern char Kind39_StartSink[];
extern char Kind39_StartStalkBelow[];
extern char Kind39_State4930[];
extern char Kind39_StateApproach[];
extern char Kind39_StateGrab[];
extern char Kind39_StatePlayOut[];
extern char Kind39_StateRise[];
extern char Kind39_StateSink[];
extern char Kind39_StateSinkBehind[];
extern char Kind39_StateSweep[];
extern char Kind39_StateUnderFloor[];
extern char Kind39_ThreatAmount[];
extern char Kind39_Update[];
extern char Kind39_dtor[];
extern char LightRing_Draw[];
extern char LightRing_SetParams[];
extern char LightRing_Start[];
extern char LightRing_Update[];
extern char LightRing_dtor[];
extern char LightShaft_Draw[];
extern char LightShaft_SetParams[];
extern char LightShaft_Start[];
extern char LightShaft_Update[];
extern char LightShaft_dtor[];
extern char LightsBase_dtor[];
extern char Lights_AddDoorway[];
extern char Lights_AreaTex0[];
extern char Lights_BlockerCount[];
extern char Lights_Blockers[];
extern char Lights_ExtraLight[];
extern char Lights_ExtraLightI[];
extern char Lights_ForModel[];
extern char Lights_Frame[];
extern char Lights_Get14[];
extern char Lights_GetLight[];
extern char Lights_RestoreLight[];
extern char Lights_Set944[];
extern char Lights_SetLight[];
extern char Lights_ShadowLights[];
extern char Lights_ShadowMayFall[];
extern char Lights_ShadowQuad[];
extern char Lights_TakeRoom[];
extern char Lights_dtor[];
extern char LoaderBase_dtor[];
extern char Loader_Cancel[];
extern char Loader_CancelAll[];
extern char Loader_CancelGroup[];
extern char Loader_CancelGroupWait[];
extern char Loader_CurrentDone[];
extern char Loader_DirListing[];
extern char Loader_FileSectors[];
extern char Loader_FileSize[];
extern char Loader_LoadNow[];
extern char Loader_Queue[];
extern char Loader_Slot24[];
extern char Loader_Slot28[];
extern char Loader_State[];
extern char Loader_dtor[];
extern char LoadingEmblem_Draw[];
extern char LoadingEmblem_dtor[];
extern char LoopingSprite_Draw[];
extern char LoopingSprite_SetParams[];
extern char LoopingSprite_Start[];
extern char LoopingSprite_Update[];
extern char LoopingSprite_dtor[];
extern char Lorenzo2Model_Frame[];
extern char Lorenzo2Model_Loaded[];
extern char Lorenzo2Model_Part0[];
extern char Lorenzo2Model_Part1[];
extern char Lorenzo2Model_Part2[];
extern char Lorenzo2Model_Part3[];
extern char Lorenzo2Model_SecondaryMotion[];
extern char Lorenzo2Model_Vt3C[];
extern char Lorenzo2Model_dtor[];
extern char Lorenzo2_ActionOffsets[];
extern char Lorenzo2_AttackAnimA[];
extern char Lorenzo2_AttackAnimB[];
extern char Lorenzo2_AttackRange[];
extern char Lorenzo2_AttackTable[];
extern char Lorenzo2_BonePositions[];
extern char Lorenzo2_ExitDone[];
extern char Lorenzo2_FrightAttack[];
extern char Lorenzo2_FrightSeen[];
extern char Lorenzo2_InStance2Anim[];
extern char Lorenzo2_ModelFiles[];
extern char Lorenzo2_MotionFiles[];
extern char Lorenzo2_ReachHewie[];
extern char Lorenzo2_SetRage[];
extern char Lorenzo2_Setup[];
extern char Lorenzo2_SlowWalkAnim[];
extern char Lorenzo2_StartGrab[];
extern char Lorenzo2_StartSink[];
extern char Lorenzo2_StartStalkBelow[];
extern char Lorenzo2_StateAB80[];
extern char Lorenzo2_StateApproach[];
extern char Lorenzo2_StateGrab[];
extern char Lorenzo2_StatePlayOut[];
extern char Lorenzo2_StateRise[];
extern char Lorenzo2_StateSink[];
extern char Lorenzo2_StateSinkBehind[];
extern char Lorenzo2_StateSweep[];
extern char Lorenzo2_StateUnderFloor[];
extern char Lorenzo2_ThreatAmount[];
extern char Lorenzo2_Update[];
extern char Lorenzo2_dtor[];
extern char LorenzoModel_AdjustBone[];
extern char LorenzoModel_BodyFrames[];
extern char LorenzoModel_Frame[];
extern char LorenzoModel_HeadBone[];
extern char LorenzoModel_HeadPos[];
extern char LorenzoModel_Loaded[];
extern char LorenzoModel_Part0[];
extern char LorenzoModel_Part1[];
extern char LorenzoModel_Part2[];
extern char LorenzoModel_Part3[];
extern char LorenzoModel_SecondaryMotion[];
extern char LorenzoModel_SetMatrix[];
extern char LorenzoModel_SlopeKeep[];
extern char LorenzoModel_Vt3C[];
extern char LorenzoModel_Vt58[];
extern char LorenzoModel_dtor[];
extern char LorenzoSpark_Draw[];
extern char LorenzoSpark_SetParams[];
extern char LorenzoSpark_Start[];
extern char LorenzoSpark_Update[];
extern char LorenzoSpark_dtor[];
extern char Lorenzo_ActionOffsets[];
extern char Lorenzo_AttackRange[];
extern char Lorenzo_AttackTable[];
extern char Lorenzo_ExitDone[];
extern char Lorenzo_Footsteps[];
extern char Lorenzo_ModelFiles[];
extern char Lorenzo_MotionFiles[];
extern char Lorenzo_ReachHewie[];
extern char Lorenzo_Setup[];
extern char Lorenzo_TurnRate[];
extern char Lorenzo_Update[];
extern char Lorenzo_dtor[];
extern char Marker_Draw[];
extern char Marker_SetParams[];
extern char Marker_Start[];
extern char Marker_State[];
extern char Marker_Update[];
extern char MemCardBase_dtor[];
extern char MemCard_Check[];
extern char MemCard_Read[];
extern char MemCard_Request10[];
extern char MemCard_Request1C[];
extern char MemCard_Request20[];
extern char MemCard_Write[];
extern char MemCard_dtor[];
extern char Message_DrawTarget[];
extern char Message_ForgetLast[];
extern char Message_Hide[];
extern char Message_ReleaseSlot[];
extern char Message_SetSlot[];
extern char Message_Show[];
extern char Message_ShowNothing[];
extern char Message_TexEntry[];
extern char Message_Unhide[];
extern char MirrorFragment_Draw[];
extern char MirrorFragment_SetParams[];
extern char MirrorFragment_Start[];
extern char MirrorFragment_Update[];
extern char MirrorFragment_dtor[];
extern char ModelBase_Loaded[];
extern char ModelBase_dtor[];
extern char ModelDrawer_dtor[];
extern char Model_AdjustBone[];
extern char Model_BodyFrames[];
extern char Model_ClearDraw[];
extern char Model_Draw[];
extern char Model_EaseTilt[];
extern char Model_FeetContact[];
extern char Model_FeetReplace[];
extern char Model_FeetUp[];
extern char Model_FloorLevel[];
extern char Model_FootDown[];
extern char Model_Frame[];
extern char Model_HalfHip[];
extern char Model_HeadBone[];
extern char Model_HeadPos[];
extern char Model_Loaded[];
extern char Model_MarkerFile[];
extern char Model_ModelFile[];
extern char Model_Part0[];
extern char Model_Part1[];
extern char Model_Part2[];
extern char Model_Part3[];
extern char Model_PlantFeet[];
extern char Model_Release[];
extern char Model_SecondaryMotion[];
extern char Model_Set878[];
extern char Model_SetMatrix[];
extern char Model_SlopeKeep[];
extern char Model_Textures[];
extern char Model_Vt1C[];
extern char Model_Vt20[];
extern char Model_Vt24[];
extern char Model_Vt2C[];
extern char Model_Vt30[];
extern char Model_Vt3C[];
extern char Model_Vt58[];
extern char Model_Vt70[];
extern char Model_Vt74[];
extern char Model_Vt78[];
extern char Model_Vt7C[];
extern char Model_Vt94[];
extern char Model_Vt98[];
extern char Model_Vt9C[];
extern char Model_VtAC[];
extern char Model_VtB0[];
extern char Model_dtor[];
extern char MovieAdded_Draw[];
extern char MovieAdded_Entry[];
extern char MovieAdded_TakeFrame[];
extern char MovieAdded_dtor[];
extern char MovieBlended_Draw[];
extern char MovieBlended_Entry[];
extern char MovieBlended_Shot[];
extern char MovieBlended_TakeFrame[];
extern char MovieBlended_dtor[];
extern char MovieCopied_Draw[];
extern char MovieCopied_Entry[];
extern char MovieCopied_Shot[];
extern char MovieCopied_TakeFrame[];
extern char MovieCopied_dtor[];
extern char MovieHalf_Draw[];
extern char MovieHalf_Entry[];
extern char MovieHalf_TakeFrame[];
extern char MovieHalf_dtor[];
extern char MovieLibBase_dtor[];
extern char MovieLib_CreatePlayer[];
extern char MovieLib_MovieSize[];
extern char MovieLib_SetWork[];
extern char MovieLib_Setup[];
extern char MovieLib_Shutdown_14[];
extern char MovieLib_WorkSize[];
extern char MovieLib_dtor[];
extern char MovieOpaque_Draw[];
extern char MovieOpaque_Entry[];
extern char MovieOpaque_Shot[];
extern char MovieOpaque_TakeFrame[];
extern char MovieOpaque_dtor[];
extern char MovieOwnBuf_Draw[];
extern char MovieOwnBuf_Entry[];
extern char MovieOwnBuf_TakeFrame[];
extern char MovieOwnBuf_dtor[];
extern char MoviePause_StateClosing[];
extern char MoviePause_StateOpen[];
extern char MoviePause_StateOpening[];
extern char MoviePause_StateSkip[];
extern char MovieScene_Entry[];
extern char MovieScene_dtor[];
extern char MovieSmall_Draw[];
extern char MovieSmall_Entry[];
extern char MovieSmall_TakeFrame[];
extern char MovieSmall_dtor[];
extern char MovieSys_dtor[];
extern char Movie_Draw[];
extern char Movie_Entry[];
extern char Movie_Finish[];
extern char Movie_Frame[];
extern char Movie_Shot[];
extern char Movie_StateOpening[];
extern char Movie_StatePausedFirst[];
extern char Movie_StateStart[];
extern char Movie_StateWaitFirst[];
extern char Movie_TakeFrame[];
extern char Movie_dtor[];
extern char MusicDirBase_dtor[];
extern char MusicDir_BanksIn[];
extern char MusicDir_BendFade[];
extern char MusicDir_ChaseBegins[];
extern char MusicDir_FullVolume[];
extern char MusicDir_Get12[];
extern char MusicDir_Get14[];
extern char MusicDir_GlobalFade[];
extern char MusicDir_GlobalVolumeTo[];
extern char MusicDir_Hold[];
extern char MusicDir_Load[];
extern char MusicDir_Lost[];
extern char MusicDir_Panic[];
extern char MusicDir_PanicOver[];
extern char MusicDir_Release[];
extern char MusicDir_Reset[];
extern char MusicDir_Seen[];
extern char MusicDir_SeqFade[];
extern char MusicDir_SetMusicVolume[];
extern char MusicDir_SetProgressVolume[];
extern char MusicDir_SetVolume[];
extern char MusicDir_Silence[];
extern char MusicDir_StageChans[];
extern char MusicDir_Start[];
extern char MusicDir_StateFrame[];
extern char MusicDir_TrackFade[];
extern char MusicDir_Volume4[];
extern char MusicDir_VolumeC[];
extern char MusicDir_dtor[];
extern char MusicStage1_Load[];
extern char MusicStage1_Lost[];
extern char MusicStage1_Seen[];
extern char MusicStage1_StageChans[];
extern char MusicStage1_dtor[];
extern char MusicStage2_Load[];
extern char MusicStage2_Lost[];
extern char MusicStage2_Seen[];
extern char MusicStage2_StageChans[];
extern char MusicStage2_dtor[];
extern char MusicStage3_Load[];
extern char MusicStage3_Lost[];
extern char MusicStage3_Seen[];
extern char MusicStage3_StageChans[];
extern char MusicStage3_dtor[];
extern char MusicStage4_Load[];
extern char MusicStage4_Lost[];
extern char MusicStage4_Seen[];
extern char MusicStage4_StageChans[];
extern char MusicStage4_dtor[];
extern char NPC_BlockFlags[];
extern char NPC_CanUseExit[];
extern char NPC_DoorBreak[];
extern char NPC_ExitArg[];
extern char NPC_FionaInReach[];
extern char NPC_GoTo[];
extern char NPC_HeadFor[];
extern char NPC_HeadForFiona[];
extern char NPC_HeadNearFiona[];
extern char NPC_HeadNearHewie[];
extern char NPC_HearNoise[];
extern char NPC_HewieInReach[];
extern char NPC_PathLengthChar[];
extern char NPC_PathLengthGoal[];
extern char NPC_PathLengthSpot[];
extern char NPC_PathLengthTo[];
extern char NPC_PickDestination[];
extern char NPC_Reset[];
extern char NPC_dtor[];
extern char NavGroupsBase_dtor[];
extern char NavGroups_ClearGroup[];
extern char NavGroups_ClearTri[];
extern char NavGroups_Holds[];
extern char NavGroups_SetGroup[];
extern char NavGroups_SetTri[];
extern char NavGroups_dtor[];
extern char NavMeshBase_dtor[];
extern char NavMesh_AtDoorRegion[];
extern char NavMesh_DoorFacing[];
extern char NavMesh_DoorSide[];
extern char NavMesh_DoorSpot[];
extern char NavMesh_DoorTri[];
extern char NavMesh_ExitEdge[];
extern char NavMesh_ExitEdgeHit[];
extern char NavMesh_FindDoorRegions[];
extern char NavMesh_FindFrom[];
extern char NavMesh_FindTri[];
extern char NavMesh_Follow[];
extern char NavMesh_HeightOver[];
extern char NavMesh_NeighbourAt[];
extern char NavMesh_OntoPlane[];
extern char NavMesh_OuterEdge[];
extern char NavMesh_PointInTri[];
extern char NavMesh_PutOnHigh[];
extern char NavMesh_PutOnLow[];
extern char NavMesh_SegmentHit[];
extern char NavMesh_SlideIn[];
extern char NavMesh_TriCentre[];
extern char NavMesh_TriNormal[];
extern char NavMesh_TriSlope[];
extern char NavMesh_Walk[];
extern char NavMesh_dtor[];
extern char Obj469A00_Noop50[];
extern char Obj46A9B0_dtor[];
extern char Obj46A9C0_dtor[];
extern char Obj46AD88_dtor[];
extern char Obj46BA80_dtor[];
extern char Obj46D780_dtor[];
extern char Obj470F90_dtor[];
extern char Obj472F60_dtor[];
extern char ObjectGlow_Draw[];
extern char ObjectGlow_SetParams[];
extern char ObjectGlow_Start[];
extern char ObjectGlow_Update[];
extern char ObjectGlow_dtor[];
extern char ObstaclesBase_dtor[];
extern char Obstacles_CanPush[];
extern char Obstacles_IsSquare[];
extern char Obstacles_KeepSpot[];
extern char Obstacles_LoadMotions[];
extern char Obstacles_ModelBack[];
extern char Obstacles_Moved[];
extern char Obstacles_PlaceAt[];
extern char Obstacles_PlaceSaved[];
extern char Obstacles_Pos[];
extern char Obstacles_Pull[];
extern char Obstacles_Push[];
extern char Obstacles_RingMask[];
extern char Obstacles_SetSaved[];
extern char Obstacles_SetTarget[];
extern char Obstacles_Settle[];
extern char Obstacles_StartMove[];
extern char Obstacles_Stop[];
extern char Obstacles_TakeSaved[];
extern char Obstacles_dtor[];
extern char OneDrip_Draw[];
extern char OneDrip_Start[];
extern char OneDrip_Update[];
extern char OneDrip_dtor[];
extern char Options_StateDefaults[];
extern char Options_StateLayout[];
extern char Options_StateList[];
extern char Options_StatePosition[];
extern char Options_StateSound[];
extern char Options_StateVibration[];
extern char Options_StateVolume[];
extern char OrangeSparks_Draw[];
extern char OrangeSparks_SetParams[];
extern char OrangeSparks_Start[];
extern char OrangeSparks_Update[];
extern char OrangeSparks_dtor[];
extern char Overlay_Draw[];
extern char PadsBase_dtor[];
extern char Pads_GetData[];
extern char Pads_SetMotors[];
extern char Pads_dtor[];
extern char Palette_Alpha11[];
extern char Palette_Colour[];
extern char Palette_FlatHigh[];
extern char Palette_FlatLow[];
extern char Palette_Grey[];
extern char Palette_RandomGrey[];
extern char Palette_Threshold[];
extern char Part50_Step[];
extern char Part60_Step[];
extern char PathPlanBase_dtor[];
extern char PathPlan_Advance[];
extern char PathPlan_AdvanceOnMesh[];
extern char PathPlan_Curve[];
extern char PathPlan_CutBack[];
extern char PathPlan_EndCentre[];
extern char PathPlan_EndTri[];
extern char PathPlan_LastLength[];
extern char PathPlan_PathLength[];
extern char PathPlan_Points[];
extern char PathPlan_PointsLength[];
extern char PathPlan_RunSearch0[];
extern char PathPlan_Start[];
extern char PathPlan_Step[];
extern char PathPlan_StepAStar[];
extern char PathPlan_StepAtGoal[];
extern char PathPlan_StepBounded[];
extern char PathPlan_StepBreadth[];
extern char PathPlan_StepCheapNear[];
extern char PathPlan_StepDepth[];
extern char PathPlan_StepDijkstra[];
extern char PathPlan_StepGreedy[];
extern char PathPlan_StepGreedySorted[];
extern char PathPlan_dtor[];
extern char Pause_StateClosing[];
extern char Pause_StateMenu[];
extern char Pause_StateMenuClosing[];
extern char Pause_StateOpen[];
extern char Pause_StateOpening[];
extern char Pause_StateQuit[];
extern char Pause_StateQuitAsk[];
extern char PlacedMesh_Draw[];
extern char PlacedMesh_dtor[];
extern char PlacedModelBase_dtor[];
extern char PlacedModel_Draw[];
extern char PlacedObjects_CallDtor[];
extern char PlacedObjects_ClearAll[];
extern char PlacedObjects_Create[];
extern char PlacedObjects_Draw[];
extern char PlacedObjects_Named[];
extern char PlacedObjects_Noop34[];
extern char PlacedObjects_RecordEntry[];
extern char PlacedObjects_ReleaseAll[];
extern char PlacedObjects_SetPair[];
extern char PlacedObjects_Sum[];
extern char PlacedObjects_Update[];
extern char PlacedThings_AnotherCame[];
extern char PlacedThings_Clear[];
extern char PlacedThings_FirstOfKind[];
extern char PlacedThings_Get[];
extern char PlacedThings_Load[];
extern char PlacedThings_New[];
extern char PlacedThings_PlaceKept[];
extern char PlacedThings_PoolCall[];
extern char PlacedThings_Save[];
extern char PlainItem_Actions[];
extern char PlainItem_IsCounted[];
extern char PlainItem_Kind[];
extern char PlainItem_dtor[];
extern char PoolEntry_Actions[];
extern char PoolEntry_AddStack[];
extern char PoolEntry_Count[];
extern char PoolEntry_CountUp[];
extern char PoolEntry_Data[];
extern char PoolEntry_Get20[];
extern char PoolEntry_Id[];
extern char PoolEntry_IsCounted[];
extern char PoolEntry_Kind[];
extern char PoolEntry_LoadPicture[];
extern char PoolEntry_PartnerCheck[];
extern char PoolEntry_SortKey[];
extern char PoolEntry_TakeOne[];
extern char PoolEntry_Use[];
extern char PoolEntry_dtor[];
extern char Progress73EC80_dtor[];
extern char ProgressBase_Slot10[];
extern char ProgressBase_Slot14[];
extern char ProgressBase_Slot18[];
extern char ProgressBase_Slot1C[];
extern char ProgressBase_Slot20[];
extern char ProgressBase_Slot24[];
extern char ProgressBase_Slot28[];
extern char ProgressBase_Slot38[];
extern char ProgressBase_Slot3C[];
extern char ProgressBase_Slot40[];
extern char ProgressBase_Slot44[];
extern char ProgressBase_Slot48[];
extern char ProgressBase_Slot4C[];
extern char ProgressBase_Slot50[];
extern char ProgressBase_Slot58[];
extern char ProgressBase_Slot5C[];
extern char ProgressBase_Slot60[];
extern char ProgressBase_Slot68[];
extern char ProgressBase_Slot6C[];
extern char ProgressBase_Slot70[];
extern char ProgressBase_Slot7C[];
extern char ProgressBase_Slot84[];
extern char ProgressBase_Slot88[];
extern char ProgressBase_SlotC[];
extern char Progress_CharBoneCall[];
extern char Progress_CharNear[];
extern char Progress_Noop54[];
extern char Progress_Noop74[];
extern char Progress_Noop78[];
extern char Progress_SlotAfresh[];
extern char Progress_StalkerAlert[];
extern char Progress_WhoIsWhere[];
extern char Progress_dtor[];
extern char Pursuer_ActionOffsets[];
extern char Pursuer_Activate[];
extern char Pursuer_AfterMove[];
extern char Pursuer_Arrived[];
extern char Pursuer_AttackAngle[];
extern char Pursuer_AttackAnimA[];
extern char Pursuer_AttackAnimB[];
extern char Pursuer_AttackNextStep[];
extern char Pursuer_AttackPoint[];
extern char Pursuer_AttackRange[];
extern char Pursuer_AttackTable[];
extern char Pursuer_BackToNormal[];
extern char Pursuer_BackToStance[];
extern char Pursuer_BackToStand[];
extern char Pursuer_Behaviour25C[];
extern char Pursuer_Behaviour288[];
extern char Pursuer_BehaviourAttack[];
extern char Pursuer_BehaviourDoors[];
extern char Pursuer_BehaviourEnded[];
extern char Pursuer_BehaviourFollow[];
extern char Pursuer_BehaviourHewie[];
extern char Pursuer_BehaviourIdle[];
extern char Pursuer_BehaviourRun[];
extern char Pursuer_BehaviourSearch[];
extern char Pursuer_BehaviourStalk[];
extern char Pursuer_BonePositions[];
extern char Pursuer_CarryOn[];
extern char Pursuer_Caught[];
extern char Pursuer_Chase1A0[];
extern char Pursuer_Chase1A4[];
extern char Pursuer_ChaseDecision[];
extern char Pursuer_ChaseFiona[];
extern char Pursuer_ChaseTarget[];
extern char Pursuer_Cleanup[];
extern char Pursuer_ClearBehaviour[];
extern char Pursuer_CloseInGoal[];
extern char Pursuer_Deactivate[];
extern char Pursuer_Disable[];
extern char Pursuer_Dist2E8[];
extern char Pursuer_Door1C4[];
extern char Pursuer_Door1C8[];
extern char Pursuer_Door1E4[];
extern char Pursuer_Door208[];
extern char Pursuer_Door214[];
extern char Pursuer_DoorAnim[];
extern char Pursuer_DoorApproach[];
extern char Pursuer_DoorArmOpen[];
extern char Pursuer_DoorBackAway[];
extern char Pursuer_DoorBackOff[];
extern char Pursuer_DoorBarge[];
extern char Pursuer_DoorFace[];
extern char Pursuer_DoorFacing[];
extern char Pursuer_DoorGoThrough[];
extern char Pursuer_DoorIdle[];
extern char Pursuer_DoorLineUp[];
extern char Pursuer_DoorOffset[];
extern char Pursuer_DoorOpen[];
extern char Pursuer_DoorPush[];
extern char Pursuer_DoorPushThrough[];
extern char Pursuer_DoorThrough[];
extern char Pursuer_DoorThroughOpen[];
extern char Pursuer_DoorWait[];
extern char Pursuer_DoorWalk[];
extern char Pursuer_DoorWalkTo[];
extern char Pursuer_DoorWalking[];
extern char Pursuer_Enable[];
extern char Pursuer_EnterRoom[];
extern char Pursuer_EventCommand[];
extern char Pursuer_EventConcerns[];
extern char Pursuer_EventOver[];
extern char Pursuer_EventOver2[];
extern char Pursuer_EventReset[];
extern char Pursuer_EventState[];
extern char Pursuer_Exit1F0[];
extern char Pursuer_ExitDone[];
extern char Pursuer_ExitGoTo[];
extern char Pursuer_ExitOpen[];
extern char Pursuer_ExitOpenStep[];
extern char Pursuer_ExitThrough[];
extern char Pursuer_FilesLoaded[];
extern char Pursuer_FilesLoading[];
extern char Pursuer_FollowPathExit[];
extern char Pursuer_FollowPlan[];
extern char Pursuer_Footsteps[];
extern char Pursuer_FrameUpdate[];
extern char Pursuer_FreshStart[];
extern char Pursuer_FrightAttack[];
extern char Pursuer_FrightSeen[];
extern char Pursuer_Get2D0[];
extern char Pursuer_Get2D4[];
extern char Pursuer_Get318[];
extern char Pursuer_GiveUp[];
extern char Pursuer_GivesUp[];
extern char Pursuer_GoAfterFiona[];
extern char Pursuer_GoForFiona[];
extern char Pursuer_GoForHewie[];
extern char Pursuer_GoToDoor[];
extern char Pursuer_GrabOrder[];
extern char Pursuer_HeadingStep[];
extern char Pursuer_InStance2Anim[];
extern char Pursuer_IsBusy[];
extern char Pursuer_KnockAtDoor[];
extern char Pursuer_LeaveScreen[];
extern char Pursuer_LeaveScreenEnding[];
extern char Pursuer_LeftBehind[];
extern char Pursuer_LightChange[];
extern char Pursuer_LoadFiles[];
extern char Pursuer_LoadMessage[];
extern char Pursuer_LoadState[];
extern char Pursuer_LocateTarget[];
extern char Pursuer_LookAround[];
extern char Pursuer_LookFrames[];
extern char Pursuer_LookSwing[];
extern char Pursuer_ModelFiles[];
extern char Pursuer_ModelUpdate[];
extern char Pursuer_Modes[];
extern char Pursuer_ModesSearching[];
extern char Pursuer_MotionFiles[];
extern char Pursuer_Move17C[];
extern char Pursuer_Move180[];
extern char Pursuer_Move184[];
extern char Pursuer_Move18C[];
extern char Pursuer_NextPathExit[];
extern char Pursuer_NoGiveUp[];
extern char Pursuer_Nothing190[];
extern char Pursuer_OffscreenStep[];
extern char Pursuer_OffscreenUpdate[];
extern char Pursuer_OnToNextExit[];
extern char Pursuer_Pace[];
extern char Pursuer_PathNodeSound[];
extern char Pursuer_PickAttack[];
extern char Pursuer_PickStep[];
extern char Pursuer_PickStepAlt[];
extern char Pursuer_PickTarget[];
extern char Pursuer_Place[];
extern char Pursuer_PlaceInRoom[];
extern char Pursuer_PlaceModel[];
extern char Pursuer_Plan16C[];
extern char Pursuer_Plan170[];
extern char Pursuer_PlanToGoal[];
extern char Pursuer_PlanWayOn[];
extern char Pursuer_ReachFiona[];
extern char Pursuer_ReachHewie[];
extern char Pursuer_ReactToEvent[];
extern char Pursuer_RememberPos[];
extern char Pursuer_Reset[];
extern char Pursuer_ResetBehaviour[];
extern char Pursuer_RoomSpots[];
extern char Pursuer_SaveState[];
extern char Pursuer_SearchRoute[];
extern char Pursuer_SetRage[];
extern char Pursuer_SetTimer[];
extern char Pursuer_Setup[];
extern char Pursuer_ShowUp[];
extern char Pursuer_SlowWalkAnim[];
extern char Pursuer_SpeedBase[];
extern char Pursuer_SpeedTop[];
extern char Pursuer_Stairs[];
extern char Pursuer_StanceAnim[];
extern char Pursuer_StandAnim[];
extern char Pursuer_StartAction[];
extern char Pursuer_StartActionNext[];
extern char Pursuer_StateArrive[];
extern char Pursuer_StateAttackActive[];
extern char Pursuer_StateAttackStep[];
extern char Pursuer_StateBackOnFeet[];
extern char Pursuer_StateBargedThrough[];
extern char Pursuer_StateCloseA[];
extern char Pursuer_StateCloseB[];
extern char Pursuer_StateCloseOnFionaA[];
extern char Pursuer_StateCloseOnFionaB[];
extern char Pursuer_StateCountKeys[];
extern char Pursuer_StateDoorAhead[];
extern char Pursuer_StateDownAndUp[];
extern char Pursuer_StateEndStep[];
extern char Pursuer_StateFaceFiona[];
extern char Pursuer_StateFlinch[];
extern char Pursuer_StateGrabStart[];
extern char Pursuer_StateHewieBites[];
extern char Pursuer_StateHewieHolds[];
extern char Pursuer_StateHitAtDoor[];
extern char Pursuer_StateHitOver[];
extern char Pursuer_StateHitReact[];
extern char Pursuer_StateHitReaction[];
extern char Pursuer_StateHurt[];
extern char Pursuer_StateKnockedDown[];
extern char Pursuer_StateKnockedThrough[];
extern char Pursuer_StateLookAround[];
extern char Pursuer_StateRelation1[];
extern char Pursuer_StateRelation2[];
extern char Pursuer_StateRootStep[];
extern char Pursuer_StateRunThenNext[];
extern char Pursuer_StateRunToTri[];
extern char Pursuer_StateSidestep[];
extern char Pursuer_StateSidestepRoom[];
extern char Pursuer_StateSpecialAnim[];
extern char Pursuer_StateStand[];
extern char Pursuer_StateStepFacing[];
extern char Pursuer_StateStepToEnd[];
extern char Pursuer_StateStopAtEnd[];
extern char Pursuer_StateTaunt[];
extern char Pursuer_StateTauntHold[];
extern char Pursuer_StateTauntRepeat[];
extern char Pursuer_StateTauntStart[];
extern char Pursuer_StateTurnAnim[];
extern char Pursuer_StateTurnThenWait[];
extern char Pursuer_StateWalkGesture[];
extern char Pursuer_StateWalkOn[];
extern char Pursuer_StateWalkThen404[];
extern char Pursuer_StateWalkThenAnim[];
extern char Pursuer_StepBack[];
extern char Pursuer_StepCount[];
extern char Pursuer_StopSounds[];
extern char Pursuer_Think[];
extern char Pursuer_Threat[];
extern char Pursuer_ThreatAmount[];
extern char Pursuer_Timer10s[];
extern char Pursuer_Timer15s[];
extern char Pursuer_TravelOffscreen[];
extern char Pursuer_TurnRate[];
extern char Pursuer_TurnRateFast[];
extern char Pursuer_Unload[];
extern char Pursuer_Update[];
extern char Pursuer_WaitRoomFlag[];
extern char Pursuer_WalkAnim[];
extern char Pursuer_WalkAside[];
extern char Pursuer_WalkToExit[];
extern char Pursuer_WalkToGoal[];
extern char Pursuer_WalkToSpot[];
extern char Pursuer_dtor[];
extern char QuadDrawer_Start[];
extern char RandomBase_dtor[];
extern char Random_Int31[];
extern char Random_Int32[];
extern char Random_Real1[];
extern char Random_Real2[];
extern char Random_Real3[];
extern char Random_Res53[];
extern char Random_Seed[];
extern char Random_dtor[];
extern char Reflection_Draw[];
extern char Reflection_SetParams[];
extern char Reflection_Start[];
extern char Reflection_Update[];
extern char RendererBase_dtor[];
extern char Renderer_AddPacket[];
extern char Renderer_Additive[];
extern char Renderer_AllocArena1[];
extern char Renderer_AllocArena2[];
extern char Renderer_BuildPalette[];
extern char Renderer_Display[];
extern char Renderer_Draw[];
extern char Renderer_DrawBox[];
extern char Renderer_FillScreen[];
extern char Renderer_Flares[];
extern char Renderer_GetColour54[];
extern char Renderer_GetTint11[];
extern char Renderer_Glow[];
extern char Renderer_GlowClear[];
extern char Renderer_LayerId[];
extern char Renderer_PacketFor[];
extern char Renderer_PutImage4[];
extern char Renderer_PutImage8[];
extern char Renderer_Quad[];
extern char Renderer_QuadArgs[];
extern char Renderer_Set304BF8[];
extern char Renderer_Set304DE0[];
extern char Renderer_SetColour54[];
extern char Renderer_SetFloorEffect[];
extern char Renderer_SetScreenOffset[];
extern char Renderer_SetSpecialColour[];
extern char Renderer_SetTint11[];
extern char Renderer_SetVideoMode[];
extern char Renderer_Sprite[];
extern char Renderer_UploadTexture[];
extern char Renderer_VideoMode[];
extern char Renderer_VramEntry[];
extern char Renderer_dtor[];
extern char Results_State[];
extern char RiccardoModel_Frame[];
extern char RiccardoModel_Loaded[];
extern char RiccardoModel_Part0[];
extern char RiccardoModel_Part1[];
extern char RiccardoModel_Part2[];
extern char RiccardoModel_Part3[];
extern char RiccardoModel_SecondaryMotion[];
extern char RiccardoModel_Vt2C[];
extern char RiccardoModel_Vt30[];
extern char RiccardoModel_Vt3C[];
extern char RiccardoModel_Vt98[];
extern char RiccardoModel_Vt9C[];
extern char RiccardoModel_dtor[];
extern char Riccardo_ActionOffsets[];
extern char Riccardo_AttackAnimA[];
extern char Riccardo_AttackAnimB[];
extern char Riccardo_AttackRange[];
extern char Riccardo_AttackTable[];
extern char Riccardo_BonePositions[];
extern char Riccardo_ChaseDecision[];
extern char Riccardo_DoorBreak[];
extern char Riccardo_DoorOffset[];
extern char Riccardo_ExitArg[];
extern char Riccardo_ExitDone[];
extern char Riccardo_FlurryBlow[];
extern char Riccardo_ModelFiles[];
extern char Riccardo_MotionFiles[];
extern char Riccardo_ReachHewie[];
extern char Riccardo_SetRage[];
extern char Riccardo_Setup[];
extern char Riccardo_SlowWalkAnim[];
extern char Riccardo_StanceAnim[];
extern char Riccardo_StartFlurry[];
extern char Riccardo_StartLunge[];
extern char Riccardo_StateAfterBlow[];
extern char Riccardo_StateBehaviour[];
extern char Riccardo_StateBlow[];
extern char Riccardo_StateBlowFiona[];
extern char Riccardo_StateBlowHewie[];
extern char Riccardo_StateChase[];
extern char Riccardo_StateLunge[];
extern char Riccardo_Update[];
extern char Riccardo_WalkAnim[];
extern char Riccardo_dtor[];
extern char RisingMotes_Draw[];
extern char RisingMotes_Start[];
extern char RisingMotes_Update[];
extern char RisingMotes_dtor[];
extern char RisingSmoke_Draw[];
extern char RisingSmoke_Start[];
extern char RisingSmoke_Update[];
extern char RisingSmoke_dtor[];
extern char Room00_ActionScript[];
extern char Room00_CharEnterScript[];
extern char Room00_Cmd00[];
extern char Room00_Cmd01[];
extern char Room00_Command[];
extern char Room00_Condition[];
extern char Room00_EnterScript[];
extern char Room00_HookNone[];
extern char Room00_ObjectName[];
extern char Room00_Phase1Script[];
extern char Room00_Phase2Script[];
extern char Room00_Phase3Script[];
extern char Room00_Table38[];
extern char Room00_dtor[];
extern char Room01_ActionScript[];
extern char Room01_CharEnterScript[];
extern char Room01_EnterScript[];
extern char Room01_Phase1Script[];
extern char Room01_Phase2Script[];
extern char Room01_Phase3Script[];
extern char Room01_Phase5Script[];
extern char Room01_Table38[];
extern char Room01_dtor[];
extern char Room02_ActionScript[];
extern char Room02_CharEnterScript[];
extern char Room02_Cmd00[];
extern char Room02_Cmd01[];
extern char Room02_Cmd02[];
extern char Room02_Cmd03[];
extern char Room02_Command[];
extern char Room02_Cond00[];
extern char Room02_Cond01[];
extern char Room02_Condition[];
extern char Room02_EnterScript[];
extern char Room02_ObjectName[];
extern char Room02_Phase1Script[];
extern char Room02_Phase2Script[];
extern char Room02_Phase3Script[];
extern char Room02_Phase5Script[];
extern char Room02_Table38[];
extern char Room02_Table3C[];
extern char Room02_dtor[];
extern char Room03_ActionScript[];
extern char Room03_CharEnterScript[];
extern char Room03_Cmd00[];
extern char Room03_Cmd01[];
extern char Room03_Command[];
extern char Room03_EnterScript[];
extern char Room03_ObjectName[];
extern char Room03_Phase1Script[];
extern char Room03_Phase2Script[];
extern char Room03_Phase3Script[];
extern char Room03_Phase5Script[];
extern char Room03_Table38[];
extern char Room03_dtor[];
extern char Room04_ActionScript[];
extern char Room04_CharEnterScript[];
extern char Room04_Cmd00[];
extern char Room04_Cmd01[];
extern char Room04_Cmd02[];
extern char Room04_Cmd03[];
extern char Room04_Command[];
extern char Room04_EnterScript[];
extern char Room04_ObjectName[];
extern char Room04_Phase1Script[];
extern char Room04_Phase2Script[];
extern char Room04_Table38[];
extern char Room04_dtor[];
extern char Room05_ActionScript[];
extern char Room05_CharEnterScript[];
extern char Room05_EnterScript[];
extern char Room05_Phase1Script[];
extern char Room05_Phase2Script[];
extern char Room05_Phase5Script[];
extern char Room05_dtor[];
extern char Room06_ActionScript[];
extern char Room06_CharEnterScript[];
extern char Room06_Cmd00[];
extern char Room06_Command[];
extern char Room06_EnterScript[];
extern char Room06_ObjectName[];
extern char Room06_Phase1Script[];
extern char Room06_Phase2Script[];
extern char Room06_Phase3Script[];
extern char Room06_Phase5Script[];
extern char Room06_dtor[];
extern char Room07_ActionScript[];
extern char Room07_CharEnterScript[];
extern char Room07_EnterScript[];
extern char Room07_Phase1Script[];
extern char Room07_Phase2Script[];
extern char Room07_Table38[];
extern char Room07_dtor[];
extern char Room08_ActionScript[];
extern char Room08_CharEnterScript[];
extern char Room08_Cmd00[];
extern char Room08_Cmd01[];
extern char Room08_Command[];
extern char Room08_Cond00[];
extern char Room08_Cond01[];
extern char Room08_Condition[];
extern char Room08_EnterScript[];
extern char Room08_ObjectName[];
extern char Room08_Phase1Script[];
extern char Room08_Phase2Script[];
extern char Room08_Phase3Script[];
extern char Room08_Phase5Script[];
extern char Room08_Table38[];
extern char Room08_dtor[];
extern char Room09_ActionScript[];
extern char Room09_CharEnterScript[];
extern char Room09_Cond00[];
extern char Room09_Condition[];
extern char Room09_EnterScript[];
extern char Room09_ObjectName[];
extern char Room09_Phase1Script[];
extern char Room09_Phase2Script[];
extern char Room09_Phase3Script[];
extern char Room09_Phase5Script[];
extern char Room09_Table38[];
extern char Room09_dtor[];
extern char Room0A_ActionScript[];
extern char Room0A_CharEnterScript[];
extern char Room0A_Cmd00[];
extern char Room0A_Cmd01[];
extern char Room0A_Command[];
extern char Room0A_EnterScript[];
extern char Room0A_ObjectName[];
extern char Room0A_Phase1Script[];
extern char Room0A_Phase2Script[];
extern char Room0A_Phase3Script[];
extern char Room0A_Table38[];
extern char Room0A_dtor[];
extern char Room0B_ActionScript[];
extern char Room0B_CharEnterScript[];
extern char Room0B_Cond00[];
extern char Room0B_Condition[];
extern char Room0B_EnterScript[];
extern char Room0B_ObjectName[];
extern char Room0B_Phase1Script[];
extern char Room0B_dtor[];
extern char Room0C_ActionScript[];
extern char Room0C_CharEnterScript[];
extern char Room0C_Cmd00[];
extern char Room0C_Cmd01[];
extern char Room0C_Cmd02[];
extern char Room0C_Cmd03[];
extern char Room0C_Command[];
extern char Room0C_EnterScript[];
extern char Room0C_ObjectName[];
extern char Room0C_Phase1Script[];
extern char Room0C_Phase2Script[];
extern char Room0C_dtor[];
extern char Room0D_ActionScript[];
extern char Room0D_CharEnterScript[];
extern char Room0D_EnterScript[];
extern char Room0D_ObjectName[];
extern char Room0D_Phase1Script[];
extern char Room0D_dtor[];
extern char Room0E_ActionScript[];
extern char Room0E_CharEnterScript[];
extern char Room0E_EnterScript[];
extern char Room0E_ObjectName[];
extern char Room0E_Phase1Script[];
extern char Room0E_Phase2Script[];
extern char Room0E_Phase3Script[];
extern char Room0E_Table38[];
extern char Room0E_dtor[];
extern char Room0F_ActionScript[];
extern char Room0F_CharEnterScript[];
extern char Room0F_Cmd00[];
extern char Room0F_Cmd01[];
extern char Room0F_Cmd02[];
extern char Room0F_Cmd03[];
extern char Room0F_Cmd04[];
extern char Room0F_Command[];
extern char Room0F_EnterScript[];
extern char Room0F_ObjectName[];
extern char Room0F_Phase1Script[];
extern char Room0F_Phase2Script[];
extern char Room0F_Table38[];
extern char Room0F_dtor[];
extern char Room100_CharEnterScript[];
extern char Room100_EnterScript[];
extern char Room100_Phase1Script[];
extern char Room100_Table38[];
extern char Room100_dtor[];
extern char Room101_CharEnterScript[];
extern char Room101_EnterScript[];
extern char Room101_Phase1Script[];
extern char Room101_Table38[];
extern char Room101_dtor[];
extern char Room102_ActionScript[];
extern char Room102_CharEnterScript[];
extern char Room102_Cmd00[];
extern char Room102_Command[];
extern char Room102_EnterScript[];
extern char Room102_ObjectName[];
extern char Room102_Phase1Script[];
extern char Room102_Phase2Script[];
extern char Room102_Table38[];
extern char Room102_dtor[];
extern char Room103_ActionScript[];
extern char Room103_CharEnterScript[];
extern char Room103_Cmd00[];
extern char Room103_Command[];
extern char Room103_EnterScript[];
extern char Room103_ObjectName[];
extern char Room103_Phase1Script[];
extern char Room103_Phase2Script[];
extern char Room103_Table38[];
extern char Room103_dtor[];
extern char Room104_ActionScript[];
extern char Room104_CharEnterScript[];
extern char Room104_Cmd00[];
extern char Room104_Command[];
extern char Room104_EnterScript[];
extern char Room104_ObjectName[];
extern char Room104_Phase1Script[];
extern char Room104_Phase2Script[];
extern char Room104_Table38[];
extern char Room104_dtor[];
extern char Room105_ActionScript[];
extern char Room105_CharEnterScript[];
extern char Room105_Cmd00[];
extern char Room105_Command[];
extern char Room105_EnterScript[];
extern char Room105_ObjectName[];
extern char Room105_Phase1Script[];
extern char Room105_Phase2Script[];
extern char Room105_Table38[];
extern char Room105_dtor[];
extern char Room106_ActionScript[];
extern char Room106_CharEnterScript[];
extern char Room106_Cmd00[];
extern char Room106_Command[];
extern char Room106_EnterScript[];
extern char Room106_ObjectName[];
extern char Room106_Phase1Script[];
extern char Room106_Phase2Script[];
extern char Room106_Table38[];
extern char Room106_dtor[];
extern char Room107_ActionScript[];
extern char Room107_CharEnterScript[];
extern char Room107_Cmd00[];
extern char Room107_Cmd01[];
extern char Room107_Command[];
extern char Room107_Cond00[];
extern char Room107_Condition[];
extern char Room107_EnterScript[];
extern char Room107_Phase1Script[];
extern char Room107_Phase2Script[];
extern char Room107_Phase5Script[];
extern char Room107_Table38[];
extern char Room107_dtor[];
extern char Room108_ActionScript[];
extern char Room108_CharEnterScript[];
extern char Room108_Cmd00[];
extern char Room108_Command[];
extern char Room108_EnterScript[];
extern char Room108_ObjectName[];
extern char Room108_Phase1Script[];
extern char Room108_Phase2Script[];
extern char Room108_Phase5Script[];
extern char Room108_Table38[];
extern char Room108_dtor[];
extern char Room109_ActionScript[];
extern char Room109_CharEnterScript[];
extern char Room109_Cmd00[];
extern char Room109_Cmd01[];
extern char Room109_Command[];
extern char Room109_EnterScript[];
extern char Room109_Phase1Script[];
extern char Room109_Phase5Script[];
extern char Room109_Table38[];
extern char Room109_dtor[];
extern char Room10A_ActionScript[];
extern char Room10A_CharEnterScript[];
extern char Room10A_Cmd00[];
extern char Room10A_Command[];
extern char Room10A_Cond00[];
extern char Room10A_Condition[];
extern char Room10A_EnterScript[];
extern char Room10A_Phase1Script[];
extern char Room10A_Phase2Script[];
extern char Room10A_Phase5Script[];
extern char Room10A_Table38[];
extern char Room10A_dtor[];
extern char Room10B_ActionScript[];
extern char Room10B_CharEnterScript[];
extern char Room10B_Cmd00[];
extern char Room10B_Command[];
extern char Room10B_Cond00[];
extern char Room10B_Condition[];
extern char Room10B_EnterScript[];
extern char Room10B_ObjectName[];
extern char Room10B_Phase1Script[];
extern char Room10B_Phase3Script[];
extern char Room10B_Table38[];
extern char Room10B_dtor[];
extern char Room10_ActionScript[];
extern char Room10_CharEnterScript[];
extern char Room10_Cond00[];
extern char Room10_Condition[];
extern char Room10_EnterScript[];
extern char Room10_ObjectName[];
extern char Room10_Phase1Script[];
extern char Room10_Phase2Script[];
extern char Room10_Phase3Script[];
extern char Room10_Phase5Script[];
extern char Room10_Table38[];
extern char Room10_dtor[];
extern char Room11_ActionScript[];
extern char Room11_CharEnterScript[];
extern char Room11_EnterScript[];
extern char Room11_ObjectName[];
extern char Room11_Phase1Script[];
extern char Room11_Phase2Script[];
extern char Room11_Table38[];
extern char Room11_dtor[];
extern char Room12_ActionScript[];
extern char Room12_CharEnterScript[];
extern char Room12_Cmd00[];
extern char Room12_Command[];
extern char Room12_Cond00[];
extern char Room12_Condition[];
extern char Room12_EnterScript[];
extern char Room12_ObjectName[];
extern char Room12_Phase1Script[];
extern char Room12_Phase2Script[];
extern char Room12_Phase3Script[];
extern char Room12_Phase5Script[];
extern char Room12_Table38[];
extern char Room12_dtor[];
extern char Room13_ActionScript[];
extern char Room13_CharEnterScript[];
extern char Room13_Cmd00[];
extern char Room13_Command[];
extern char Room13_EnterScript[];
extern char Room13_ObjectName[];
extern char Room13_Phase1Script[];
extern char Room13_Phase2Script[];
extern char Room13_Phase3Script[];
extern char Room13_Table38[];
extern char Room13_dtor[];
extern char Room14_ActionScript[];
extern char Room14_CharEnterScript[];
extern char Room14_Cmd00[];
extern char Room14_Command[];
extern char Room14_EnterScript[];
extern char Room14_ObjectName[];
extern char Room14_Phase1Script[];
extern char Room14_Phase2Script[];
extern char Room14_Table38[];
extern char Room14_dtor[];
extern char Room15_ActionScript[];
extern char Room15_CharEnterScript[];
extern char Room15_Cmd00[];
extern char Room15_Command[];
extern char Room15_Cond00[];
extern char Room15_Condition[];
extern char Room15_EnterScript[];
extern char Room15_ObjectName[];
extern char Room15_Phase1Script[];
extern char Room15_Phase2Script[];
extern char Room15_Phase3Script[];
extern char Room15_Table38[];
extern char Room15_dtor[];
extern char Room16_ActionScript[];
extern char Room16_CharEnterScript[];
extern char Room16_EnterScript[];
extern char Room16_Phase1Script[];
extern char Room16_Phase2Script[];
extern char Room16_Phase3Script[];
extern char Room16_Table38[];
extern char Room16_dtor[];
extern char Room17_ActionScript[];
extern char Room17_CharEnterScript[];
extern char Room17_EnterScript[];
extern char Room17_ObjectName[];
extern char Room17_Phase1Script[];
extern char Room17_Phase2Script[];
extern char Room17_dtor[];
extern char Room18_ActionScript[];
extern char Room18_CharEnterScript[];
extern char Room18_Cmd00[];
extern char Room18_Command[];
extern char Room18_EnterScript[];
extern char Room18_ObjectName[];
extern char Room18_Phase1Script[];
extern char Room18_Phase2Script[];
extern char Room18_Phase3Script[];
extern char Room18_Phase5Script[];
extern char Room18_Table38[];
extern char Room18_dtor[];
extern char Room19_ActionScript[];
extern char Room19_CharEnterScript[];
extern char Room19_Cond00[];
extern char Room19_Condition[];
extern char Room19_EnterScript[];
extern char Room19_ObjectName[];
extern char Room19_Phase1Script[];
extern char Room19_Phase2Script[];
extern char Room19_Phase3Script[];
extern char Room19_Table38[];
extern char Room19_dtor[];
extern char Room1A_ActionScript[];
extern char Room1A_CharEnterScript[];
extern char Room1A_Cmd00[];
extern char Room1A_Cmd01[];
extern char Room1A_Command[];
extern char Room1A_EnterScript[];
extern char Room1A_ObjectName[];
extern char Room1A_Phase1Script[];
extern char Room1A_Phase2Script[];
extern char Room1A_Phase3Script[];
extern char Room1A_Table38[];
extern char Room1A_dtor[];
extern char Room1B_ActionScript[];
extern char Room1B_CharEnterScript[];
extern char Room1B_Cmd00[];
extern char Room1B_Command[];
extern char Room1B_EnterScript[];
extern char Room1B_ObjectName[];
extern char Room1B_Phase1Script[];
extern char Room1B_Phase2Script[];
extern char Room1B_Table38[];
extern char Room1B_dtor[];
extern char Room1C_ActionScript[];
extern char Room1C_CharEnterScript[];
extern char Room1C_Cmd00[];
extern char Room1C_Cmd01[];
extern char Room1C_Command[];
extern char Room1C_EnterScript[];
extern char Room1C_ObjectName[];
extern char Room1C_Phase1Script[];
extern char Room1C_Phase2Script[];
extern char Room1C_Table38[];
extern char Room1C_dtor[];
extern char Room1D_ActionScript[];
extern char Room1D_CharEnterScript[];
extern char Room1D_Cmd00[];
extern char Room1D_Cmd01[];
extern char Room1D_Command[];
extern char Room1D_Cond00[];
extern char Room1D_Condition[];
extern char Room1D_EnterScript[];
extern char Room1D_Phase1Script[];
extern char Room1D_Phase2Script[];
extern char Room1D_Phase3Script[];
extern char Room1D_Phase5Script[];
extern char Room1D_Table38[];
extern char Room1D_dtor[];
extern char Room1E_ActionScript[];
extern char Room1E_CharEnterScript[];
extern char Room1E_EnterScript[];
extern char Room1E_ObjectName[];
extern char Room1E_Phase1Script[];
extern char Room1E_Phase2Script[];
extern char Room1E_Phase5Script[];
extern char Room1E_Table38[];
extern char Room1E_dtor[];
extern char Room1F_ActionScript[];
extern char Room1F_CharEnterScript[];
extern char Room1F_Cmd00[];
extern char Room1F_Command[];
extern char Room1F_EnterScript[];
extern char Room1F_ObjectName[];
extern char Room1F_Phase1Script[];
extern char Room1F_Phase2Script[];
extern char Room1F_Table38[];
extern char Room1F_dtor[];
extern char Room20_ActionScript[];
extern char Room20_CharEnterScript[];
extern char Room20_Cmd00[];
extern char Room20_Cmd01[];
extern char Room20_Cmd02[];
extern char Room20_Cmd03[];
extern char Room20_Cmd04[];
extern char Room20_Cmd05[];
extern char Room20_Command[];
extern char Room20_Cond00[];
extern char Room20_Cond01[];
extern char Room20_Cond02[];
extern char Room20_Condition[];
extern char Room20_EnterScript[];
extern char Room20_ObjectName[];
extern char Room20_Phase1Script[];
extern char Room20_Phase2Script[];
extern char Room20_Phase3Script[];
extern char Room20_Phase5Script[];
extern char Room20_Table38[];
extern char Room20_Table3C[];
extern char Room20_dtor[];
extern char Room21_ActionScript[];
extern char Room21_CharEnterScript[];
extern char Room21_Cmd00[];
extern char Room21_Cmd01[];
extern char Room21_Cmd02[];
extern char Room21_Cmd03[];
extern char Room21_Cmd04[];
extern char Room21_Cmd05[];
extern char Room21_Cmd06[];
extern char Room21_Command[];
extern char Room21_Cond00[];
extern char Room21_Condition[];
extern char Room21_EnterScript[];
extern char Room21_ObjectName[];
extern char Room21_Phase1Script[];
extern char Room21_Phase2Script[];
extern char Room21_Phase3Script[];
extern char Room21_Table38[];
extern char Room21_dtor[];
extern char Room22_ActionScript[];
extern char Room22_CharEnterScript[];
extern char Room22_EnterScript[];
extern char Room22_ObjectName[];
extern char Room22_Phase1Script[];
extern char Room22_Phase2Script[];
extern char Room22_Phase3Script[];
extern char Room22_Table38[];
extern char Room22_dtor[];
extern char Room23_ActionScript[];
extern char Room23_CharEnterScript[];
extern char Room23_Cmd00[];
extern char Room23_Cmd01[];
extern char Room23_Command[];
extern char Room23_Cond00[];
extern char Room23_Condition[];
extern char Room23_EnterScript[];
extern char Room23_ObjectName[];
extern char Room23_Phase1Script[];
extern char Room23_Phase2Script[];
extern char Room23_Table38[];
extern char Room23_dtor[];
extern char Room24_ActionScript[];
extern char Room24_CharEnterScript[];
extern char Room24_ColourPulse[];
extern char Room24_Command[];
extern char Room24_EnterScript[];
extern char Room24_KilnGlow[];
extern char Room24_ObjectName[];
extern char Room24_Phase1Script[];
extern char Room24_Phase2Script[];
extern char Room24_Phase5Script[];
extern char Room24_Smoke[];
extern char Room24_Table38[];
extern char Room24_dtor[];
extern char Room25_ActionScript[];
extern char Room25_CharEnterScript[];
extern char Room25_Cmd00[];
extern char Room25_Command[];
extern char Room25_Cond00[];
extern char Room25_Condition[];
extern char Room25_EnterScript[];
extern char Room25_ObjectName[];
extern char Room25_Phase1Script[];
extern char Room25_Phase2Script[];
extern char Room25_Phase3Script[];
extern char Room25_Phase5Script[];
extern char Room25_Table38[];
extern char Room25_dtor[];
extern char Room26_ActionScript[];
extern char Room26_CharEnterScript[];
extern char Room26_Cmd00[];
extern char Room26_Cmd01[];
extern char Room26_Cmd02[];
extern char Room26_Command[];
extern char Room26_Cond00[];
extern char Room26_Condition[];
extern char Room26_EnterScript[];
extern char Room26_ObjectName[];
extern char Room26_Phase1Script[];
extern char Room26_Phase2Script[];
extern char Room26_Table38[];
extern char Room26_dtor[];
extern char Room27_ActionScript[];
extern char Room27_CharEnterScript[];
extern char Room27_Cmd00[];
extern char Room27_Command[];
extern char Room27_EnterScript[];
extern char Room27_Phase1Script[];
extern char Room27_Phase2Script[];
extern char Room27_dtor[];
extern char Room28_ActionScript[];
extern char Room28_CharEnterScript[];
extern char Room28_Cond00[];
extern char Room28_Condition[];
extern char Room28_EnterScript[];
extern char Room28_ObjectName[];
extern char Room28_Phase1Script[];
extern char Room28_Phase2Script[];
extern char Room28_Phase5Script[];
extern char Room28_Table38[];
extern char Room28_dtor[];
extern char Room29_ActionScript[];
extern char Room29_CharEnterScript[];
extern char Room29_Cond00[];
extern char Room29_Cond01[];
extern char Room29_Condition[];
extern char Room29_EnterScript[];
extern char Room29_ObjectName[];
extern char Room29_Phase1Script[];
extern char Room29_Phase2Script[];
extern char Room29_Phase5Script[];
extern char Room29_Table38[];
extern char Room29_dtor[];
extern char Room2AWisps_Draw[];
extern char Room2AWisps_SetParams[];
extern char Room2AWisps_Start[];
extern char Room2AWisps_Update[];
extern char Room2AWisps_dtor[];
extern char Room2A_ActionScript[];
extern char Room2A_CharEnterScript[];
extern char Room2A_Cmd00[];
extern char Room2A_Cmd02[];
extern char Room2A_Cmd03[];
extern char Room2A_Command[];
extern char Room2A_EnterScript[];
extern char Room2A_HandlerStep[];
extern char Room2A_ObjectName[];
extern char Room2A_Phase1Script[];
extern char Room2A_Phase2Script[];
extern char Room2A_Phase5Script[];
extern char Room2A_Table38[];
extern char Room2A_dtor[];
extern char Room2B_ActionScript[];
extern char Room2B_CharEnterScript[];
extern char Room2B_Cmd00[];
extern char Room2B_Command[];
extern char Room2B_EnterScript[];
extern char Room2B_Phase1Script[];
extern char Room2B_Phase2Script[];
extern char Room2B_Table38[];
extern char Room2B_dtor[];
extern char Room2C_ActionScript[];
extern char Room2C_CharEnterScript[];
extern char Room2C_EnterScript[];
extern char Room2C_ObjectName[];
extern char Room2C_Phase1Script[];
extern char Room2C_dtor[];
extern char Room2D_ActionScript[];
extern char Room2D_CharEnterScript[];
extern char Room2D_Cmd00[];
extern char Room2D_Cmd01[];
extern char Room2D_Cmd02[];
extern char Room2D_Command[];
extern char Room2D_Cond00[];
extern char Room2D_Condition[];
extern char Room2D_EnterScript[];
extern char Room2D_ObjectName[];
extern char Room2D_Phase1Script[];
extern char Room2D_Phase2Script[];
extern char Room2D_Table38[];
extern char Room2D_dtor[];
extern char Room2E_ActionScript[];
extern char Room2E_CharEnterScript[];
extern char Room2E_Cmd00[];
extern char Room2E_Cmd01[];
extern char Room2E_Cmd02[];
extern char Room2E_Cmd03[];
extern char Room2E_Cmd04[];
extern char Room2E_Command[];
extern char Room2E_EnterScript[];
extern char Room2E_ObjectName[];
extern char Room2E_Phase1Script[];
extern char Room2E_Phase2Script[];
extern char Room2E_Phase5Script[];
extern char Room2E_dtor[];
extern char Room2F_ActionScript[];
extern char Room2F_CharEnterScript[];
extern char Room2F_Cond00[];
extern char Room2F_Condition[];
extern char Room2F_EnterScript[];
extern char Room2F_ObjectName[];
extern char Room2F_Phase1Script[];
extern char Room2F_Phase2Script[];
extern char Room2F_Phase5Script[];
extern char Room2F_Table38[];
extern char Room2F_dtor[];
extern char Room30_ActionScript[];
extern char Room30_CharEnterScript[];
extern char Room30_Cond00[];
extern char Room30_Condition[];
extern char Room30_EnterScript[];
extern char Room30_ObjectName[];
extern char Room30_Phase1Script[];
extern char Room30_Phase2Script[];
extern char Room30_Phase5Script[];
extern char Room30_Table38[];
extern char Room30_dtor[];
extern char Room31_ActionScript[];
extern char Room31_CharEnterScript[];
extern char Room31_Command[];
extern char Room31_EnterScript[];
extern char Room31_Fan[];
extern char Room31_ObjectName[];
extern char Room31_Phase1Script[];
extern char Room31_RoomEffect[];
extern char Room31_WindowLight[];
extern char Room31_dtor[];
extern char Room32_ActionScript[];
extern char Room32_CharEnterScript[];
extern char Room32_Cmd00[];
extern char Room32_Cmd01[];
extern char Room32_Cmd02[];
extern char Room32_Cmd03[];
extern char Room32_Cmd04[];
extern char Room32_Cmd05[];
extern char Room32_Command[];
extern char Room32_EnterScript[];
extern char Room32_ObjectName[];
extern char Room32_Phase1Script[];
extern char Room32_Phase2Script[];
extern char Room32_Table38[];
extern char Room32_dtor[];
extern char Room33_ActionScript[];
extern char Room33_CharEnterScript[];
extern char Room33_EnterScript[];
extern char Room33_ObjectName[];
extern char Room33_Phase1Script[];
extern char Room33_dtor[];
extern char Room34_ActionScript[];
extern char Room34_CharEnterScript[];
extern char Room34_EnterScript[];
extern char Room34_ObjectName[];
extern char Room34_Phase1Script[];
extern char Room34_dtor[];
extern char Room35_ActionScript[];
extern char Room35_CharEnterScript[];
extern char Room35_EnterScript[];
extern char Room35_ObjectName[];
extern char Room35_dtor[];
extern char Room36_ActionScript[];
extern char Room36_CharEnterScript[];
extern char Room36_EnterScript[];
extern char Room36_ObjectName[];
extern char Room36_dtor[];
extern char Room37_ActionScript[];
extern char Room37_CharEnterScript[];
extern char Room37_Command[];
extern char Room37_EnterScript[];
extern char Room37_Fan[];
extern char Room37_ObjectName[];
extern char Room37_Phase1Script[];
extern char Room37_Phase2Script[];
extern char Room37_Phase3Script[];
extern char Room37_dtor[];
extern char Room40_ActionScript[];
extern char Room40_CharEnterScript[];
extern char Room40_Cmd00[];
extern char Room40_Command[];
extern char Room40_EnterScript[];
extern char Room40_ObjectName[];
extern char Room40_Phase1Script[];
extern char Room40_Phase2Script[];
extern char Room40_Table38[];
extern char Room40_dtor[];
extern char Room41_ActionScript[];
extern char Room41_CharEnterScript[];
extern char Room41_EnterScript[];
extern char Room41_Phase1Script[];
extern char Room41_Phase2Script[];
extern char Room41_Phase3Script[];
extern char Room41_Phase5Script[];
extern char Room41_Table38[];
extern char Room41_dtor[];
extern char Room42_ActionScript[];
extern char Room42_CharEnterScript[];
extern char Room42_EnterScript[];
extern char Room42_Phase1Script[];
extern char Room42_Phase2Script[];
extern char Room42_Table38[];
extern char Room42_dtor[];
extern char Room43_ActionScript[];
extern char Room43_CharEnterScript[];
extern char Room43_Cmd00[];
extern char Room43_Command[];
extern char Room43_EnterScript[];
extern char Room43_Phase1Script[];
extern char Room43_Phase2Script[];
extern char Room43_Phase5Script[];
extern char Room43_Table38[];
extern char Room43_dtor[];
extern char Room44_ActionScript[];
extern char Room44_CharEnterScript[];
extern char Room44_EnterScript[];
extern char Room44_Phase1Script[];
extern char Room44_Phase2Script[];
extern char Room44_Phase3Script[];
extern char Room44_Phase5Script[];
extern char Room44_Table38[];
extern char Room44_dtor[];
extern char Room45_ActionScript[];
extern char Room45_CharEnterScript[];
extern char Room45_Cmd00[];
extern char Room45_Command[];
extern char Room45_EnterScript[];
extern char Room45_ObjectName[];
extern char Room45_Phase1Script[];
extern char Room45_Phase2Script[];
extern char Room45_Phase5Script[];
extern char Room45_Table38[];
extern char Room45_dtor[];
extern char Room46_ActionScript[];
extern char Room46_CharEnterScript[];
extern char Room46_CharHook[];
extern char Room46_Command[];
extern char Room46_Condition[];
extern char Room46_EnterScript[];
extern char Room46_ObjectName[];
extern char Room46_Phase1Script[];
extern char Room46_Phase2Script[];
extern char Room46_Phase3Script[];
extern char Room46_Phase5Script[];
extern char Room46_PursuerBusy[];
extern char Room46_Table38[];
extern char Room46_dtor[];
extern char Room47_ActionScript[];
extern char Room47_CharEnterScript[];
extern char Room47_EnterScript[];
extern char Room47_ObjectName[];
extern char Room47_Phase1Script[];
extern char Room47_Phase2Script[];
extern char Room47_Table38[];
extern char Room47_dtor[];
extern char Room48_ActionScript[];
extern char Room48_CharEnterScript[];
extern char Room48_Cmd00[];
extern char Room48_Cmd01[];
extern char Room48_Cmd02[];
extern char Room48_Cmd03[];
extern char Room48_Cmd04[];
extern char Room48_Cmd05[];
extern char Room48_Command[];
extern char Room48_Cond00[];
extern char Room48_Condition[];
extern char Room48_EnterScript[];
extern char Room48_ObjectName[];
extern char Room48_Phase1Script[];
extern char Room48_Phase2Script[];
extern char Room48_Phase5Script[];
extern char Room48_Table38[];
extern char Room48_dtor[];
extern char Room49Effect_Draw[];
extern char Room49Effect_Start[];
extern char Room49Effect_Update[];
extern char Room49Effect_dtor[];
extern char Room49_ActionScript[];
extern char Room49_CharEnterScript[];
extern char Room49_Cmd00[];
extern char Room49_Cmd01[];
extern char Room49_Cmd02[];
extern char Room49_Command[];
extern char Room49_EnterScript[];
extern char Room49_ObjectName[];
extern char Room49_Phase1Script[];
extern char Room49_Phase2Script[];
extern char Room49_Table38[];
extern char Room49_dtor[];
extern char Room4A_ActionScript[];
extern char Room4A_CharEnterScript[];
extern char Room4A_EnterScript[];
extern char Room4A_ObjectName[];
extern char Room4A_Phase1Script[];
extern char Room4A_Phase2Script[];
extern char Room4A_Phase5Script[];
extern char Room4A_Table38[];
extern char Room4A_dtor[];
extern char Room4B_ActionScript[];
extern char Room4B_CharEnterScript[];
extern char Room4B_Cmd00[];
extern char Room4B_Cmd01[];
extern char Room4B_Command[];
extern char Room4B_Cond00[];
extern char Room4B_Condition[];
extern char Room4B_EnterScript[];
extern char Room4B_ObjectName[];
extern char Room4B_Phase1Script[];
extern char Room4B_Phase2Script[];
extern char Room4B_Phase3Script[];
extern char Room4B_Phase5Script[];
extern char Room4B_Table38[];
extern char Room4B_dtor[];
extern char Room4C_ActionScript[];
extern char Room4C_CharEnterScript[];
extern char Room4C_Cmd00[];
extern char Room4C_Cmd01[];
extern char Room4C_Cmd02[];
extern char Room4C_Command[];
extern char Room4C_EnterScript[];
extern char Room4C_ObjectName[];
extern char Room4C_Phase1Script[];
extern char Room4C_Phase2Script[];
extern char Room4C_Table38[];
extern char Room4C_dtor[];
extern char Room4D_CharEnterScript[];
extern char Room4D_EnterScript[];
extern char Room4D_Phase1Script[];
extern char Room4D_Table38[];
extern char Room4D_dtor[];
extern char Room4EEffect_Draw[];
extern char Room4EEffect_Start[];
extern char Room4EEffect_Update[];
extern char Room4EEffect_dtor[];
extern char Room4E_ActionScript[];
extern char Room4E_CharEnterScript[];
extern char Room4E_Cmd00[];
extern char Room4E_Cmd01[];
extern char Room4E_Cmd02[];
extern char Room4E_Command[];
extern char Room4E_EnterScript[];
extern char Room4E_ObjectName[];
extern char Room4E_Phase1Script[];
extern char Room4E_Phase2Script[];
extern char Room4E_Phase5Script[];
extern char Room4E_Table38[];
extern char Room4E_dtor[];
extern char Room4F_ActionScript[];
extern char Room4F_CharEnterScript[];
extern char Room4F_Cmd00[];
extern char Room4F_Cmd01[];
extern char Room4F_Cmd02[];
extern char Room4F_Cmd03[];
extern char Room4F_Command[];
extern char Room4F_Condition[];
extern char Room4F_CreatureVanish[];
extern char Room4F_EnterScript[];
extern char Room4F_ObjectName[];
extern char Room4F_Phase1Script[];
extern char Room4F_Phase2Script[];
extern char Room4F_Table38[];
extern char Room4F_dtor[];
extern char Room50_ActionScript[];
extern char Room50_CharEnterScript[];
extern char Room50_Cmd00[];
extern char Room50_Command[];
extern char Room50_EnterScript[];
extern char Room50_ObjectName[];
extern char Room50_Phase1Script[];
extern char Room50_Phase2Script[];
extern char Room50_Phase5Script[];
extern char Room50_Table38[];
extern char Room50_dtor[];
extern char Room51_ActionScript[];
extern char Room51_CharEnterScript[];
extern char Room51_Cmd00[];
extern char Room51_Cmd01[];
extern char Room51_Command[];
extern char Room51_EnterScript[];
extern char Room51_ObjectName[];
extern char Room51_Phase1Script[];
extern char Room51_Phase2Script[];
extern char Room51_Phase5Script[];
extern char Room51_Table38[];
extern char Room51_dtor[];
extern char Room52_ActionScript[];
extern char Room52_CharEnterScript[];
extern char Room52_Cmd00[];
extern char Room52_Command[];
extern char Room52_Cond00[];
extern char Room52_Condition[];
extern char Room52_EnterScript[];
extern char Room52_ObjectName[];
extern char Room52_Phase1Script[];
extern char Room52_Phase2Script[];
extern char Room52_Table38[];
extern char Room52_dtor[];
extern char Room53_ActionScript[];
extern char Room53_CharEnterScript[];
extern char Room53_EnterScript[];
extern char Room53_ObjectName[];
extern char Room53_Phase1Script[];
extern char Room53_Phase2Script[];
extern char Room53_Table38[];
extern char Room53_dtor[];
extern char Room54_ActionScript[];
extern char Room54_CharEnterScript[];
extern char Room54_Command[];
extern char Room54_DoorRegions[];
extern char Room54_EnterScript[];
extern char Room54_GroupStep[];
extern char Room54_ObjectName[];
extern char Room54_Phase1Script[];
extern char Room54_Phase2Script[];
extern char Room54_Table38[];
extern char Room54_dtor[];
extern char Room55Effect_Draw[];
extern char Room55Effect_SetParams[];
extern char Room55Effect_Start[];
extern char Room55Effect_Update[];
extern char Room55Effect_dtor[];
extern char Room55_ActionScript[];
extern char Room55_CharEnterScript[];
extern char Room55_Command[];
extern char Room55_Effect[];
extern char Room55_EnterScript[];
extern char Room55_ObjectName[];
extern char Room55_Phase1Script[];
extern char Room55_Phase2Script[];
extern char Room55_Table38[];
extern char Room55_dtor[];
extern char Room56_ActionScript[];
extern char Room56_CharEnterScript[];
extern char Room56_Cmd00[];
extern char Room56_Command[];
extern char Room56_EnterScript[];
extern char Room56_Phase1Script[];
extern char Room56_Phase2Script[];
extern char Room56_Phase3Script[];
extern char Room56_Table38[];
extern char Room56_dtor[];
extern char Room57_ActionScript[];
extern char Room57_CharEnterScript[];
extern char Room57_EnterScript[];
extern char Room57_ObjectName[];
extern char Room57_Phase1Script[];
extern char Room57_Phase2Script[];
extern char Room57_Phase5Script[];
extern char Room57_Table38[];
extern char Room57_dtor[];
extern char Room58_ActionScript[];
extern char Room58_CharEnterScript[];
extern char Room58_EnterScript[];
extern char Room58_ObjectName[];
extern char Room58_Phase1Script[];
extern char Room58_Phase2Script[];
extern char Room58_Phase5Script[];
extern char Room58_Table38[];
extern char Room58_dtor[];
extern char Room59_ActionScript[];
extern char Room59_CharEnterScript[];
extern char Room59_Cmd00[];
extern char Room59_Command[];
extern char Room59_EnterScript[];
extern char Room59_ObjectName[];
extern char Room59_Phase1Script[];
extern char Room59_Phase2Script[];
extern char Room59_Table38[];
extern char Room59_dtor[];
extern char Room5A_ActionScript[];
extern char Room5A_CharEnterScript[];
extern char Room5A_Cmd00[];
extern char Room5A_Command[];
extern char Room5A_EnterScript[];
extern char Room5A_ObjectName[];
extern char Room5A_Phase1Script[];
extern char Room5A_Phase2Script[];
extern char Room5A_Phase5Script[];
extern char Room5A_Table38[];
extern char Room5A_dtor[];
extern char Room5B_CharEnterScript[];
extern char Room5B_EnterScript[];
extern char Room5B_Phase1Script[];
extern char Room5B_Table38[];
extern char Room5B_dtor[];
extern char Room5C_ActionScript[];
extern char Room5C_CharEnterScript[];
extern char Room5C_Cmd00[];
extern char Room5C_Command[];
extern char Room5C_Cond00[];
extern char Room5C_Condition[];
extern char Room5C_EnterScript[];
extern char Room5C_ObjectName[];
extern char Room5C_Phase1Script[];
extern char Room5C_Phase2Script[];
extern char Room5C_Phase3Script[];
extern char Room5C_Table38[];
extern char Room5C_dtor[];
extern char Room5D_ActionScript[];
extern char Room5D_CharEnterScript[];
extern char Room5D_Cmd00[];
extern char Room5D_Cmd01[];
extern char Room5D_Command[];
extern char Room5D_EnterScript[];
extern char Room5D_ObjectName[];
extern char Room5D_Phase1Script[];
extern char Room5D_Phase2Script[];
extern char Room5D_Table38[];
extern char Room5D_dtor[];
extern char Room5E_ActionScript[];
extern char Room5E_CharEnterScript[];
extern char Room5E_EnterScript[];
extern char Room5E_Phase1Script[];
extern char Room5E_Phase2Script[];
extern char Room5E_Table38[];
extern char Room5E_dtor[];
extern char Room5F_ActionScript[];
extern char Room5F_CharEnterScript[];
extern char Room5F_EnterScript[];
extern char Room5F_Phase1Script[];
extern char Room5F_Phase2Script[];
extern char Room5F_Phase3Script[];
extern char Room5F_Table38[];
extern char Room5F_dtor[];
extern char Room60Effect_Draw[];
extern char Room60Effect_SetParams[];
extern char Room60Effect_Start[];
extern char Room60Effect_Update[];
extern char Room60Effect_dtor[];
extern char Room60_ActionScript[];
extern char Room60_CharEnterScript[];
extern char Room60_Cmd00[];
extern char Room60_Cmd01[];
extern char Room60_Cmd02[];
extern char Room60_Cmd03[];
extern char Room60_Cmd04[];
extern char Room60_Cmd05[];
extern char Room60_Command[];
extern char Room60_EnterScript[];
extern char Room60_ObjectName[];
extern char Room60_Phase1Script[];
extern char Room60_Phase2Script[];
extern char Room60_Phase3Script[];
extern char Room60_Table38[];
extern char Room60_dtor[];
extern char Room61_ActionScript[];
extern char Room61_CharEnterScript[];
extern char Room61_Cmd00[];
extern char Room61_Cmd01[];
extern char Room61_Command[];
extern char Room61_EnterScript[];
extern char Room61_Phase1Script[];
extern char Room61_Phase2Script[];
extern char Room61_Phase3Script[];
extern char Room61_Phase5Script[];
extern char Room61_Table38[];
extern char Room61_dtor[];
extern char Room62_ActionScript[];
extern char Room62_CharEnterScript[];
extern char Room62_Cmd00[];
extern char Room62_Cmd01[];
extern char Room62_Command[];
extern char Room62_EnterScript[];
extern char Room62_ObjectName[];
extern char Room62_Phase1Script[];
extern char Room62_Phase2Script[];
extern char Room62_Phase3Script[];
extern char Room62_Table38[];
extern char Room62_dtor[];
extern char Room63_ActionScript[];
extern char Room63_CharEnterScript[];
extern char Room63_Cmd00[];
extern char Room63_Command[];
extern char Room63_EnterScript[];
extern char Room63_ObjectName[];
extern char Room63_Phase1Script[];
extern char Room63_Phase2Script[];
extern char Room63_Phase5Script[];
extern char Room63_Table38[];
extern char Room63_dtor[];
extern char Room64_CharEnterScript[];
extern char Room64_EnterScript[];
extern char Room64_Phase1Script[];
extern char Room64_Table38[];
extern char Room64_dtor[];
extern char Room65_CharEnterScript[];
extern char Room65_EnterScript[];
extern char Room65_Phase1Script[];
extern char Room65_Table38[];
extern char Room65_dtor[];
extern char Room66Effect_Draw[];
extern char Room66Effect_SetParams[];
extern char Room66Effect_Start[];
extern char Room66Effect_Update[];
extern char Room66Effect_dtor[];
extern char Room66_ActionScript[];
extern char Room66_CharEnterScript[];
extern char Room66_CharHook[];
extern char Room66_CharPast[];
extern char Room66_ColourPulse[];
extern char Room66_Command[];
extern char Room66_Condition[];
extern char Room66_Effect[];
extern char Room66_EnterScript[];
extern char Room66_Fire[];
extern char Room66_Letters[];
extern char Room66_ObjectName[];
extern char Room66_Phase1Script[];
extern char Room66_Phase2Script[];
extern char Room66_Phase5Script[];
extern char Room66_Sink[];
extern char Room66_Table38[];
extern char Room66_dtor[];
extern char Room67_ActionScript[];
extern char Room67_CharEnterScript[];
extern char Room67_EnterScript[];
extern char Room67_Phase1Script[];
extern char Room67_Phase2Script[];
extern char Room67_Phase5Script[];
extern char Room67_Table38[];
extern char Room67_dtor[];
extern char Room68_ActionScript[];
extern char Room68_CharEnterScript[];
extern char Room68_EnterScript[];
extern char Room68_ObjectName[];
extern char Room68_Phase1Script[];
extern char Room68_Phase2Script[];
extern char Room68_Phase4Script[];
extern char Room68_dtor[];
extern char Room69_ActionScript[];
extern char Room69_CharEnterScript[];
extern char Room69_Cmd00[];
extern char Room69_Command[];
extern char Room69_EnterScript[];
extern char Room69_ObjectName[];
extern char Room69_Phase1Script[];
extern char Room69_Phase2Script[];
extern char Room69_Phase3Script[];
extern char Room69_Phase5Script[];
extern char Room69_Table38[];
extern char Room69_dtor[];
extern char Room6A_ActionScript[];
extern char Room6A_CharEnterScript[];
extern char Room6A_Cmd00[];
extern char Room6A_Cmd01[];
extern char Room6A_Command[];
extern char Room6A_EnterScript[];
extern char Room6A_ObjectName[];
extern char Room6A_Phase1Script[];
extern char Room6A_Phase2Script[];
extern char Room6A_Phase5Script[];
extern char Room6A_Table38[];
extern char Room6A_dtor[];
extern char Room6B_ActionScript[];
extern char Room6B_CharEnterScript[];
extern char Room6B_EnterScript[];
extern char Room6B_ObjectName[];
extern char Room6B_Phase1Script[];
extern char Room6B_Phase2Script[];
extern char Room6B_Phase5Script[];
extern char Room6B_Table38[];
extern char Room6B_dtor[];
extern char Room6C_ActionScript[];
extern char Room6C_CharEnterScript[];
extern char Room6C_Cond00[];
extern char Room6C_Cond01[];
extern char Room6C_Condition[];
extern char Room6C_EnterScript[];
extern char Room6C_ObjectName[];
extern char Room6C_Phase1Script[];
extern char Room6C_Phase2Script[];
extern char Room6C_Phase5Script[];
extern char Room6C_Table38[];
extern char Room6C_dtor[];
extern char Room6D_ActionScript[];
extern char Room6D_CharEnterScript[];
extern char Room6D_Cond00[];
extern char Room6D_Cond01[];
extern char Room6D_Condition[];
extern char Room6D_EnterScript[];
extern char Room6D_ObjectName[];
extern char Room6D_Phase1Script[];
extern char Room6D_Phase2Script[];
extern char Room6D_Phase3Script[];
extern char Room6D_Table38[];
extern char Room6D_dtor[];
extern char Room6E_ActionScript[];
extern char Room6E_CharEnterScript[];
extern char Room6E_EnterScript[];
extern char Room6E_ObjectName[];
extern char Room6E_Phase1Script[];
extern char Room6E_Phase2Script[];
extern char Room6E_Phase3Script[];
extern char Room6E_Table38[];
extern char Room6E_dtor[];
extern char Room6F_ActionScript[];
extern char Room6F_CharEnterScript[];
extern char Room6F_Cmd00[];
extern char Room6F_Command[];
extern char Room6F_Cond00[];
extern char Room6F_Condition[];
extern char Room6F_EnterScript[];
extern char Room6F_ObjectName[];
extern char Room6F_Phase1Script[];
extern char Room6F_Phase2Script[];
extern char Room6F_Phase5Script[];
extern char Room6F_Table38[];
extern char Room6F_dtor[];
extern char Room70_ActionScript[];
extern char Room70_CharEnterScript[];
extern char Room70_EnterScript[];
extern char Room70_ObjectName[];
extern char Room70_Phase1Script[];
extern char Room70_Phase2Script[];
extern char Room70_Phase5Script[];
extern char Room70_Table38[];
extern char Room70_dtor[];
extern char Room80_ActionScript[];
extern char Room80_CharEnterScript[];
extern char Room80_Cmd00[];
extern char Room80_Command[];
extern char Room80_EnterScript[];
extern char Room80_ObjectName[];
extern char Room80_Phase1Script[];
extern char Room80_Phase2Script[];
extern char Room80_dtor[];
extern char Room81_ActionScript[];
extern char Room81_CharEnterScript[];
extern char Room81_EnterScript[];
extern char Room81_ObjectName[];
extern char Room81_Phase1Script[];
extern char Room81_Phase2Script[];
extern char Room81_Phase3Script[];
extern char Room81_Phase5Script[];
extern char Room81_Table38[];
extern char Room81_dtor[];
extern char Room82_ActionScript[];
extern char Room82_CharEnterScript[];
extern char Room82_EnterScript[];
extern char Room82_Phase1Script[];
extern char Room82_Phase2Script[];
extern char Room82_Table38[];
extern char Room82_dtor[];
extern char Room83_ActionScript[];
extern char Room83_CharEnterScript[];
extern char Room83_EnterScript[];
extern char Room83_ObjectName[];
extern char Room83_Phase1Script[];
extern char Room83_Phase2Script[];
extern char Room83_Phase3Script[];
extern char Room83_Table38[];
extern char Room83_dtor[];
extern char Room84_ActionScript[];
extern char Room84_CharEnterScript[];
extern char Room84_Cmd00[];
extern char Room84_Command[];
extern char Room84_EnterScript[];
extern char Room84_ObjectName[];
extern char Room84_Phase1Script[];
extern char Room84_Phase2Script[];
extern char Room84_Table38[];
extern char Room84_dtor[];
extern char Room85_ActionScript[];
extern char Room85_CharEnterScript[];
extern char Room85_Cmd00[];
extern char Room85_Command[];
extern char Room85_EnterScript[];
extern char Room85_ObjectName[];
extern char Room85_Phase1Script[];
extern char Room85_Phase2Script[];
extern char Room85_Table38[];
extern char Room85_dtor[];
extern char Room86_ActionScript[];
extern char Room86_CharEnterScript[];
extern char Room86_Cmd00[];
extern char Room86_Command[];
extern char Room86_EnterScript[];
extern char Room86_ObjectName[];
extern char Room86_Phase1Script[];
extern char Room86_Table38[];
extern char Room86_dtor[];
extern char Room87_ActionScript[];
extern char Room87_CharEnterScript[];
extern char Room87_Cmd00[];
extern char Room87_Command[];
extern char Room87_EnterScript[];
extern char Room87_Phase1Script[];
extern char Room87_Phase2Script[];
extern char Room87_Phase5Script[];
extern char Room87_dtor[];
extern char Room88_ActionScript[];
extern char Room88_CharEnterScript[];
extern char Room88_Cmd00[];
extern char Room88_Command[];
extern char Room88_EnterScript[];
extern char Room88_ObjectName[];
extern char Room88_Phase1Script[];
extern char Room88_Phase2Script[];
extern char Room88_Phase3Script[];
extern char Room88_Table38[];
extern char Room88_dtor[];
extern char Room8A_ActionScript[];
extern char Room8A_CharEnterScript[];
extern char Room8A_EnterScript[];
extern char Room8A_ObjectName[];
extern char Room8A_Phase1Script[];
extern char Room8A_Phase2Script[];
extern char Room8A_Table38[];
extern char Room8A_dtor[];
extern char Room8C_ActionScript[];
extern char Room8C_CharEnterScript[];
extern char Room8C_Cmd00[];
extern char Room8C_Cmd01[];
extern char Room8C_Cmd02[];
extern char Room8C_Cmd03[];
extern char Room8C_Cmd04[];
extern char Room8C_Command[];
extern char Room8C_EnterScript[];
extern char Room8C_ObjectName[];
extern char Room8C_Phase1Script[];
extern char Room8C_Phase2Script[];
extern char Room8C_Phase5Script[];
extern char Room8C_Table38[];
extern char Room8C_dtor[];
extern char Room8D_ActionScript[];
extern char Room8D_CharEnterScript[];
extern char Room8D_Cmd00[];
extern char Room8D_Cmd01[];
extern char Room8D_Cmd02[];
extern char Room8D_Command[];
extern char Room8D_EnterScript[];
extern char Room8D_ObjectName[];
extern char Room8D_Phase1Script[];
extern char Room8D_Phase2Script[];
extern char Room8D_Phase3Script[];
extern char Room8D_Phase5Script[];
extern char Room8D_Table38[];
extern char Room8D_dtor[];
extern char Room8E_ActionScript[];
extern char Room8E_CharEnterScript[];
extern char Room8E_Cmd00[];
extern char Room8E_Cmd01[];
extern char Room8E_Command[];
extern char Room8E_EnterScript[];
extern char Room8E_ObjectName[];
extern char Room8E_Phase1Script[];
extern char Room8E_Phase2Script[];
extern char Room8E_dtor[];
extern char Room8F_ActionScript[];
extern char Room8F_CharEnterScript[];
extern char Room8F_Cmd00[];
extern char Room8F_Cmd01[];
extern char Room8F_Cmd02[];
extern char Room8F_Cmd03[];
extern char Room8F_Command[];
extern char Room8F_Cond00[];
extern char Room8F_Condition[];
extern char Room8F_EnterScript[];
extern char Room8F_ObjectName[];
extern char Room8F_Phase1Script[];
extern char Room8F_Phase2Script[];
extern char Room8F_Phase3Script[];
extern char Room8F_Table38[];
extern char Room8F_dtor[];
extern char Room91_ActionScript[];
extern char Room91_CharEnterScript[];
extern char Room91_Cmd00[];
extern char Room91_Cmd01[];
extern char Room91_Cmd02[];
extern char Room91_Cmd03[];
extern char Room91_Command[];
extern char Room91_EnterScript[];
extern char Room91_Phase1Script[];
extern char Room91_Phase2Script[];
extern char Room91_Phase3Script[];
extern char Room91_Table38[];
extern char Room91_dtor[];
extern char Room92Effect_Draw[];
extern char Room92Effect_SetParams[];
extern char Room92Effect_Start[];
extern char Room92Effect_Update[];
extern char Room92Effect_dtor[];
extern char Room92_ActionScript[];
extern char Room92_CharEnterScript[];
extern char Room92_Cmd00[];
extern char Room92_Cmd01[];
extern char Room92_Cmd02[];
extern char Room92_Cmd03[];
extern char Room92_Cmd04[];
extern char Room92_Cmd05[];
extern char Room92_Cmd06[];
extern char Room92_Cmd07[];
extern char Room92_Cmd08[];
extern char Room92_Cmd09[];
extern char Room92_Cmd10[];
extern char Room92_Cmd11[];
extern char Room92_Cmd12[];
extern char Room92_Cmd13[];
extern char Room92_Command[];
extern char Room92_EnterScript[];
extern char Room92_ObjectName[];
extern char Room92_Phase1Script[];
extern char Room92_Phase2Script[];
extern char Room92_Phase3Script[];
extern char Room92_dtor[];
extern char Room93_ActionScript[];
extern char Room93_CharEnterScript[];
extern char Room93_Command[];
extern char Room93_EnterScript[];
extern char Room93_Phase1Script[];
extern char Room93_Phase2Script[];
extern char Room93_Phase5Script[];
extern char Room93_SlamShake[];
extern char Room93_dtor[];
extern char Room94_ActionScript[];
extern char Room94_CharEnterScript[];
extern char Room94_Cmd00[];
extern char Room94_Command[];
extern char Room94_EnterScript[];
extern char Room94_Phase1Script[];
extern char Room94_Phase2Script[];
extern char Room94_Phase5Script[];
extern char Room94_dtor[];
extern char Room95_ActionScript[];
extern char Room95_CharEnterScript[];
extern char Room95_Cmd00[];
extern char Room95_Command[];
extern char Room95_EnterScript[];
extern char Room95_Phase1Script[];
extern char Room95_Phase2Script[];
extern char Room95_Phase5Script[];
extern char Room95_dtor[];
extern char Room96_ActionScript[];
extern char Room96_CharEnterScript[];
extern char Room96_Cmd00[];
extern char Room96_Command[];
extern char Room96_EnterScript[];
extern char Room96_Phase1Script[];
extern char Room96_Phase2Script[];
extern char Room96_Phase5Script[];
extern char Room96_dtor[];
extern char Room97_ActionScript[];
extern char Room97_CharEnterScript[];
extern char Room97_Cmd00[];
extern char Room97_Command[];
extern char Room97_EnterScript[];
extern char Room97_Phase1Script[];
extern char Room97_Phase2Script[];
extern char Room97_Table38[];
extern char Room97_dtor[];
extern char Room98_ActionScript[];
extern char Room98_CharEnterScript[];
extern char Room98_Command[];
extern char Room98_EnterScript[];
extern char Room98_Phase1Script[];
extern char Room98_Phase2Script[];
extern char Room98_SlamShake[];
extern char Room98_Table38[];
extern char Room98_dtor[];
extern char Room99_ActionScript[];
extern char Room99_CharEnterScript[];
extern char Room99_Command[];
extern char Room99_EnterScript[];
extern char Room99_Phase1Script[];
extern char Room99_Phase2Script[];
extern char Room99_Phase5Script[];
extern char Room99_SlamShake[];
extern char Room99_Table38[];
extern char Room99_dtor[];
extern char Room9A_ActionScript[];
extern char Room9A_CharEnterScript[];
extern char Room9A_Cmd00[];
extern char Room9A_Command[];
extern char Room9A_EnterScript[];
extern char Room9A_Phase1Script[];
extern char Room9A_Phase2Script[];
extern char Room9A_Table38[];
extern char Room9A_dtor[];
extern char Room9B_ActionScript[];
extern char Room9B_CharEnterScript[];
extern char Room9B_Cmd00[];
extern char Room9B_Command[];
extern char Room9B_EnterScript[];
extern char Room9B_ObjectName[];
extern char Room9B_Phase1Script[];
extern char Room9B_Phase2Script[];
extern char Room9B_Phase3Script[];
extern char Room9B_Table38[];
extern char Room9B_dtor[];
extern char RoomBase_ActionScript[];
extern char RoomBase_CharEnterScript[];
extern char RoomBase_Command[];
extern char RoomBase_Condition[];
extern char RoomBase_EnterScript[];
extern char RoomBase_ObjectName[];
extern char RoomBase_Phase1Script[];
extern char RoomBase_Phase2Script[];
extern char RoomBase_Phase3Script[];
extern char RoomBase_Phase4Script[];
extern char RoomBase_Phase5Script[];
extern char RoomBase_Table38[];
extern char RoomBase_Table3C[];
extern char RoomBase_dtor[];
extern char RoomC0_ActionScript[];
extern char RoomC0_CharEnterScript[];
extern char RoomC0_Cmd00[];
extern char RoomC0_Cmd01[];
extern char RoomC0_Cmd02[];
extern char RoomC0_Cmd03[];
extern char RoomC0_Cmd04[];
extern char RoomC0_Command[];
extern char RoomC0_Cond00[];
extern char RoomC0_Condition[];
extern char RoomC0_EnterScript[];
extern char RoomC0_ObjectName[];
extern char RoomC0_Phase1Script[];
extern char RoomC0_Phase2Script[];
extern char RoomC0_Table38[];
extern char RoomC0_dtor[];
extern char RoomC1_ActionScript[];
extern char RoomC1_CharEnterScript[];
extern char RoomC1_Cond00[];
extern char RoomC1_Condition[];
extern char RoomC1_EnterScript[];
extern char RoomC1_ObjectName[];
extern char RoomC1_Phase1Script[];
extern char RoomC1_Phase2Script[];
extern char RoomC1_Phase3Script[];
extern char RoomC1_Table38[];
extern char RoomC1_dtor[];
extern char RoomC2_ActionScript[];
extern char RoomC2_CharEnterScript[];
extern char RoomC2_Cmd00[];
extern char RoomC2_Cmd01[];
extern char RoomC2_Command[];
extern char RoomC2_Cond00[];
extern char RoomC2_Condition[];
extern char RoomC2_EnterScript[];
extern char RoomC2_ObjectName[];
extern char RoomC2_Phase1Script[];
extern char RoomC2_Phase2Script[];
extern char RoomC2_Phase5Script[];
extern char RoomC2_Table38[];
extern char RoomC2_dtor[];
extern char RoomC3_ActionScript[];
extern char RoomC3_CharEnterScript[];
extern char RoomC3_Cmd00[];
extern char RoomC3_Cmd01[];
extern char RoomC3_Cmd02[];
extern char RoomC3_Command[];
extern char RoomC3_Cond00[];
extern char RoomC3_Condition[];
extern char RoomC3_EnterScript[];
extern char RoomC3_ObjectName[];
extern char RoomC3_Phase1Script[];
extern char RoomC3_Phase2Script[];
extern char RoomC3_Phase5Script[];
extern char RoomC3_Table38[];
extern char RoomC3_dtor[];
extern char RoomC4_ActionScript[];
extern char RoomC4_CharEnterScript[];
extern char RoomC4_EnterScript[];
extern char RoomC4_ObjectName[];
extern char RoomC4_Phase1Script[];
extern char RoomC4_Table38[];
extern char RoomC4_dtor[];
extern char RoomC5_ActionScript[];
extern char RoomC5_CharEnterScript[];
extern char RoomC5_Cmd00[];
extern char RoomC5_Command[];
extern char RoomC5_EnterScript[];
extern char RoomC5_ObjectName[];
extern char RoomC5_Phase1Script[];
extern char RoomC5_Phase2Script[];
extern char RoomC5_Phase5Script[];
extern char RoomC5_Table38[];
extern char RoomC5_dtor[];
extern char RoomC6_ActionScript[];
extern char RoomC6_CharEnterScript[];
extern char RoomC6_EnterScript[];
extern char RoomC6_ObjectName[];
extern char RoomC6_Phase1Script[];
extern char RoomC6_Phase2Script[];
extern char RoomC6_Phase5Script[];
extern char RoomC6_Table38[];
extern char RoomC6_dtor[];
extern char RoomC7Cursor_Draw[];
extern char RoomC7Cursor_SetParams[];
extern char RoomC7Cursor_Start[];
extern char RoomC7Cursor_Update[];
extern char RoomC7Cursor_dtor[];
extern char RoomC7_ActionScript[];
extern char RoomC7_CharEnterScript[];
extern char RoomC7_Cmd00[];
extern char RoomC7_Cmd01[];
extern char RoomC7_Cmd02[];
extern char RoomC7_Cmd03[];
extern char RoomC7_Cmd04[];
extern char RoomC7_Cmd05[];
extern char RoomC7_Cmd06[];
extern char RoomC7_Command[];
extern char RoomC7_Cond00[];
extern char RoomC7_Cond01[];
extern char RoomC7_Condition[];
extern char RoomC7_EnterScript[];
extern char RoomC7_ObjectName[];
extern char RoomC7_Phase1Script[];
extern char RoomC7_Phase2Script[];
extern char RoomC7_Phase3Script[];
extern char RoomC7_Phase5Script[];
extern char RoomC7_Table38[];
extern char RoomC7_dtor[];
extern char RoomC8_ActionScript[];
extern char RoomC8_CharEnterScript[];
extern char RoomC8_Cmd00[];
extern char RoomC8_Command[];
extern char RoomC8_EnterScript[];
extern char RoomC8_ObjectName[];
extern char RoomC8_Phase1Script[];
extern char RoomC8_Phase2Script[];
extern char RoomC8_Table38[];
extern char RoomC8_dtor[];
extern char RoomD0_ActionScript[];
extern char RoomD0_CharEnterScript[];
extern char RoomD0_ClockDrawSaved[];
extern char RoomD0_ClockSave[];
extern char RoomD0_ClockStop[];
extern char RoomD0_Command[];
extern char RoomD0_Condition[];
extern char RoomD0_EnterScript[];
extern char RoomD0_Phase1Script[];
extern char RoomD0_Phase2Script[];
extern char RoomD0_Phase3Script[];
extern char RoomD0_ProgressCall[];
extern char RoomD0_Table38[];
extern char RoomD0_dtor[];
extern char RoomD1_ActionScript[];
extern char RoomD1_CharEnterScript[];
extern char RoomD1_Cmd00[];
extern char RoomD1_Cmd01[];
extern char RoomD1_Command[];
extern char RoomD1_Cond00[];
extern char RoomD1_Condition[];
extern char RoomD1_EnterScript[];
extern char RoomD1_Phase1Script[];
extern char RoomD1_Phase2Script[];
extern char RoomD1_Phase3Script[];
extern char RoomD1_Table38[];
extern char RoomD1_dtor[];
extern char RoomD2_CharEnterScript[];
extern char RoomD2_Cmd00[];
extern char RoomD2_Command[];
extern char RoomD2_Cond00[];
extern char RoomD2_Condition[];
extern char RoomD2_EnterScript[];
extern char RoomD2_Phase1Script[];
extern char RoomD2_Phase3Script[];
extern char RoomD2_Table38[];
extern char RoomD2_dtor[];
extern char RoomD3_ActionScript[];
extern char RoomD3_CharEnterScript[];
extern char RoomD3_Cmd00[];
extern char RoomD3_Command[];
extern char RoomD3_Cond00[];
extern char RoomD3_Condition[];
extern char RoomD3_EnterScript[];
extern char RoomD3_Phase1Script[];
extern char RoomD3_Phase2Script[];
extern char RoomD3_Phase3Script[];
extern char RoomD3_Table38[];
extern char RoomD3_dtor[];
extern char RoomD4_CharEnterScript[];
extern char RoomD4_Cmd00[];
extern char RoomD4_Command[];
extern char RoomD4_Cond00[];
extern char RoomD4_Condition[];
extern char RoomD4_EnterScript[];
extern char RoomD4_Phase1Script[];
extern char RoomD4_Phase3Script[];
extern char RoomD4_Table38[];
extern char RoomD4_dtor[];
extern char RoomD5_ActionScript[];
extern char RoomD5_CharEnterScript[];
extern char RoomD5_Cmd00[];
extern char RoomD5_Cmd01[];
extern char RoomD5_Cmd02[];
extern char RoomD5_Command[];
extern char RoomD5_Cond00[];
extern char RoomD5_Condition[];
extern char RoomD5_EnterScript[];
extern char RoomD5_Phase1Script[];
extern char RoomD5_Phase3Script[];
extern char RoomD5_Phase5Script[];
extern char RoomD5_Table38[];
extern char RoomD5_dtor[];
extern char RoomD6_ActionScript[];
extern char RoomD6_CharEnterScript[];
extern char RoomD6_Cmd00[];
extern char RoomD6_Cmd01[];
extern char RoomD6_Command[];
extern char RoomD6_Cond00[];
extern char RoomD6_Condition[];
extern char RoomD6_EnterScript[];
extern char RoomD6_ObjectName[];
extern char RoomD6_Phase1Script[];
extern char RoomD6_Phase2Script[];
extern char RoomD6_Phase3Script[];
extern char RoomD6_Phase5Script[];
extern char RoomD6_Table38[];
extern char RoomD6_dtor[];
extern char RoomD7_ActionScript[];
extern char RoomD7_CharEnterScript[];
extern char RoomD7_Cmd00[];
extern char RoomD7_Cmd01[];
extern char RoomD7_Cmd02[];
extern char RoomD7_Command[];
extern char RoomD7_Cond00[];
extern char RoomD7_Cond01[];
extern char RoomD7_Condition[];
extern char RoomD7_EnterScript[];
extern char RoomD7_Phase1Script[];
extern char RoomD7_Phase2Script[];
extern char RoomD7_Phase3Script[];
extern char RoomD7_Phase5Script[];
extern char RoomD7_Table38[];
extern char RoomD7_dtor[];
extern char RoomD8_ActionScript[];
extern char RoomD8_CharEnterScript[];
extern char RoomD8_Cmd00[];
extern char RoomD8_Cmd01[];
extern char RoomD8_Command[];
extern char RoomD8_Cond00[];
extern char RoomD8_Cond01[];
extern char RoomD8_Condition[];
extern char RoomD8_EnterScript[];
extern char RoomD8_Phase1Script[];
extern char RoomD8_Phase2Script[];
extern char RoomD8_Phase3Script[];
extern char RoomD8_Phase5Script[];
extern char RoomD8_Table38[];
extern char RoomD8_dtor[];
extern char RoomD9_ActionScript[];
extern char RoomD9_CharEnterScript[];
extern char RoomD9_Cmd00[];
extern char RoomD9_Cmd01[];
extern char RoomD9_Cmd02[];
extern char RoomD9_Command[];
extern char RoomD9_Cond00[];
extern char RoomD9_Condition[];
extern char RoomD9_EnterScript[];
extern char RoomD9_Phase1Script[];
extern char RoomD9_Phase3Script[];
extern char RoomD9_Table38[];
extern char RoomD9_dtor[];
extern char RoomE0_ActionScript[];
extern char RoomE0_CharEnterScript[];
extern char RoomE0_ChooseExit[];
extern char RoomE0_ClockDraw[];
extern char RoomE0_ClockStart[];
extern char RoomE0_Command[];
extern char RoomE0_Condition[];
extern char RoomE0_DoorLight[];
extern char RoomE0_EnterScript[];
extern char RoomE0_Fan[];
extern char RoomE0_ObjectName[];
extern char RoomE0_Phase1Script[];
extern char RoomE0_Phase2Script[];
extern char RoomE0_Phase3Script[];
extern char RoomE0_Phase5Script[];
extern char RoomE0_ProgressCall[];
extern char RoomE0_Table38[];
extern char RoomE0_WindowLight[];
extern char RoomE0_dtor[];
extern char RoomE1_ActionScript[];
extern char RoomE1_CharEnterScript[];
extern char RoomE1_Cmd00[];
extern char RoomE1_Command[];
extern char RoomE1_Cond00[];
extern char RoomE1_Condition[];
extern char RoomE1_EnterScript[];
extern char RoomE1_Phase1Script[];
extern char RoomE1_Phase2Script[];
extern char RoomE1_Phase3Script[];
extern char RoomE1_Table38[];
extern char RoomE1_dtor[];
extern char RoomE2_ActionScript[];
extern char RoomE2_CharEnterScript[];
extern char RoomE2_Cmd00[];
extern char RoomE2_Command[];
extern char RoomE2_Cond00[];
extern char RoomE2_Condition[];
extern char RoomE2_EnterScript[];
extern char RoomE2_Phase1Script[];
extern char RoomE2_Phase2Script[];
extern char RoomE2_Phase3Script[];
extern char RoomE2_Table38[];
extern char RoomE2_dtor[];
extern char RoomE3_ActionScript[];
extern char RoomE3_CharEnterScript[];
extern char RoomE3_Cmd00[];
extern char RoomE3_Command[];
extern char RoomE3_Cond00[];
extern char RoomE3_Condition[];
extern char RoomE3_EnterScript[];
extern char RoomE3_ObjectName[];
extern char RoomE3_Phase1Script[];
extern char RoomE3_Phase2Script[];
extern char RoomE3_Phase3Script[];
extern char RoomE3_Phase5Script[];
extern char RoomE3_Table38[];
extern char RoomE3_dtor[];
extern char RoomE4_ActionScript[];
extern char RoomE4_CharEnterScript[];
extern char RoomE4_Cmd00[];
extern char RoomE4_Cmd01[];
extern char RoomE4_Command[];
extern char RoomE4_Cond00[];
extern char RoomE4_Condition[];
extern char RoomE4_EnterScript[];
extern char RoomE4_ObjectName[];
extern char RoomE4_Phase1Script[];
extern char RoomE4_Phase2Script[];
extern char RoomE4_Phase3Script[];
extern char RoomE4_Table38[];
extern char RoomE4_dtor[];
extern char RoomE5_ActionScript[];
extern char RoomE5_CharEnterScript[];
extern char RoomE5_Cmd00[];
extern char RoomE5_Cmd01[];
extern char RoomE5_Command[];
extern char RoomE5_Cond00[];
extern char RoomE5_Condition[];
extern char RoomE5_EnterScript[];
extern char RoomE5_ObjectName[];
extern char RoomE5_Phase1Script[];
extern char RoomE5_Phase2Script[];
extern char RoomE5_Phase3Script[];
extern char RoomE5_Phase5Script[];
extern char RoomE5_Table38[];
extern char RoomE5_dtor[];
extern char RoomE6_ActionScript[];
extern char RoomE6_CharEnterScript[];
extern char RoomE6_Cmd00[];
extern char RoomE6_Cmd01[];
extern char RoomE6_Command[];
extern char RoomE6_Cond00[];
extern char RoomE6_Condition[];
extern char RoomE6_EnterScript[];
extern char RoomE6_ObjectName[];
extern char RoomE6_Phase1Script[];
extern char RoomE6_Phase2Script[];
extern char RoomE6_Phase3Script[];
extern char RoomE6_Table38[];
extern char RoomE6_dtor[];
extern char RoomE7_ActionScript[];
extern char RoomE7_CharEnterScript[];
extern char RoomE7_Cmd00[];
extern char RoomE7_Command[];
extern char RoomE7_Cond00[];
extern char RoomE7_Condition[];
extern char RoomE7_EnterScript[];
extern char RoomE7_ObjectName[];
extern char RoomE7_Phase1Script[];
extern char RoomE7_Phase2Script[];
extern char RoomE7_Phase3Script[];
extern char RoomE7_Phase5Script[];
extern char RoomE7_Table38[];
extern char RoomE7_dtor[];
extern char RoomE8_ActionScript[];
extern char RoomE8_CharEnterScript[];
extern char RoomE8_Cmd00[];
extern char RoomE8_Command[];
extern char RoomE8_Cond00[];
extern char RoomE8_Condition[];
extern char RoomE8_EnterScript[];
extern char RoomE8_Phase1Script[];
extern char RoomE8_Phase2Script[];
extern char RoomE8_Phase3Script[];
extern char RoomE8_Table38[];
extern char RoomE8_dtor[];
extern char RoomE9_ActionScript[];
extern char RoomE9_CharEnterScript[];
extern char RoomE9_Cmd00[];
extern char RoomE9_Cmd01[];
extern char RoomE9_Cmd02[];
extern char RoomE9_Cmd03[];
extern char RoomE9_Command[];
extern char RoomE9_Cond00[];
extern char RoomE9_Condition[];
extern char RoomE9_EnterScript[];
extern char RoomE9_Phase1Script[];
extern char RoomE9_Phase2Script[];
extern char RoomE9_Phase3Script[];
extern char RoomE9_Table38[];
extern char RoomE9_dtor[];
extern char RoomEffectBase_Noop18[];
extern char RoomEffectBase_dtor[];
extern char RoomMesh_Draw[];
extern char RoomMeshes_dtor[];
extern char Room_EmptyHook[];
extern char RoomsBase_dtor[];
extern char Rooms_CameraArea[];
extern char Rooms_Centre[];
extern char Rooms_CloseOff[];
extern char Rooms_Distance[];
extern char Rooms_DoorClosedOff[];
extern char Rooms_DoorExit[];
extern char Rooms_DoorFlags[];
extern char Rooms_DoorFromOther[];
extern char Rooms_DoorFromSide[];
extern char Rooms_DoorLeadsTo[];
extern char Rooms_DoorSideRoom[];
extern char Rooms_DoorTri[];
extern char Rooms_ExitDoor[];
extern char Rooms_ExitDoorFlags[];
extern char Rooms_ExitFromOther[];
extern char Rooms_ExitHasSideFlag[];
extern char Rooms_ExitHeading[];
extern char Rooms_ExitLeads[];
extern char Rooms_ExitLoops[];
extern char Rooms_ExitNotLocked[];
extern char Rooms_ExitPointIn[];
extern char Rooms_ExitPointOut[];
extern char Rooms_ExitPointThrough[];
extern char Rooms_ExitSideFlag[];
extern char Rooms_ExitTriIn[];
extern char Rooms_ExitTriOut[];
extern char Rooms_ExitTriThrough[];
extern char Rooms_NeighbourCount[];
extern char Rooms_OtherExit[];
extern char Rooms_OtherRoom[];
extern char Rooms_Rebuild[];
extern char Rooms_Reopen[];
extern char Rooms_SavedState[];
extern char Rooms_SetState[];
extern char Rooms_dtor[];
extern char RoutePlannerBase_dtor[];
extern char RoutePlanner_FindRoute[];
extern char RoutePlanner_dtor[];
extern char RumbleBase_dtor[];
extern char Rumble_Clear[];
extern char Rumble_Enable[];
extern char Rumble_ListRunning[];
extern char Rumble_MoveB[];
extern char Rumble_Reset[];
extern char Rumble_Set2C[];
extern char Rumble_SetA[];
extern char Rumble_SetB[];
extern char Rumble_StartLists[];
extern char Rumble_dtor[];
extern char SceneBoot_StateDone[];
extern char SceneBoot_StateEntry[];
extern char SceneBoot_StateLoadSystem[];
extern char SceneBoot_StateSequence[];
extern char SceneBoot_StepButton[];
extern char SceneBoot_StepCapcom[];
extern char SceneBoot_StepCaution[];
extern char SceneBoot_StepCri[];
extern char SceneBoot_StepDolby[];
extern char SceneBoot_StepObject[];
extern char SceneBoot_StepProgressive[];
extern char SceneBoot_StepTexts[];
extern char SceneBoot_dtor[];
extern char SceneEnding_Finish[];
extern char SceneEnding_SeqResults[];
extern char SceneEnding_SeqSave[];
extern char SceneEnding_SeqSetup[];
extern char SceneEnding_SeqTitleMessage[];
extern char SceneEnding_SeqUnlocks[];
extern char SceneEnding_StateEntry[];
extern char SceneEnding_StateResults[];
extern char SceneEnding_StateStaffFade[];
extern char SceneEnding_StateStaffRoll[];
extern char SceneEnding_StateStart[];
extern char SceneEnding_dtor[];
extern char SceneGame_Bits4to9[];
extern char SceneGame_BonusFlagCount[];
extern char SceneGame_BonusFlagsSet[];
extern char SceneGame_Busy[];
extern char SceneGame_CurrentRoom[];
extern char SceneGame_EndMusic[];
extern char SceneGame_EventCamRunning[];
extern char SceneGame_InPlay[];
extern char SceneGame_LeaveRoom[];
extern char SceneGame_LoadSpareRoom[];
extern char SceneGame_LoadSpareSlot[];
extern char SceneGame_LockRoomDoors[];
extern char SceneGame_MusicDirector[];
extern char SceneGame_OnSoftReset[];
extern char SceneGame_PlaceCreature[];
extern char SceneGame_PlayStart[];
extern char SceneGame_PrepareExit[];
extern char SceneGame_RandomRoomNear[];
extern char SceneGame_RoomClosedTo[];
extern char SceneGame_RoomsNear[];
extern char SceneGame_Save[];
extern char SceneGame_SetCreatureCount[];
extern char SceneGame_SetFlag3[];
extern char SceneGame_SetRoomBit[];
extern char SceneGame_SetRoomDoors[];
extern char SceneGame_SpareRoomData[];
extern char SceneGame_State7[];
extern char SceneGame_StateEntry[];
extern char SceneGame_StateMain[];
extern char SceneGame_StatePlay[];
extern char SceneGame_SubMoviePaused[];
extern char SceneGame_SubPaused[];
extern char SceneGame_SubPlay[];
extern char SceneGame_SubScreenStart[];
extern char SceneGame_SubStartRoom[];
extern char SceneGame_SubSubScreen[];
extern char SceneGame_SubTransition[];
extern char SceneGame_SubWaitRoom[];
extern char SceneGame_UnlockRoomDoors[];
extern char SceneGame_Update[];
extern char SceneGame_dtor[];
extern char SceneTableBase_dtor[];
extern char SceneTable_dtor[];
extern char SceneTitle_Finish[];
extern char SceneTitle_SeqFadeIn[];
extern char SceneTitle_SeqLeave[];
extern char SceneTitle_SeqLoad[];
extern char SceneTitle_SeqLoadGame[];
extern char SceneTitle_SeqMenu[];
extern char SceneTitle_SeqMenuToTitle[];
extern char SceneTitle_SeqOptions[];
extern char SceneTitle_SeqPressStart[];
extern char SceneTitle_SeqToMenu[];
extern char SceneTitle_StateAttract[];
extern char SceneTitle_StateAttractFade[];
extern char SceneTitle_StateAttractStart[];
extern char SceneTitle_StateDemo[];
extern char SceneTitle_StateDemoFade[];
extern char SceneTitle_StateDemoStart[];
extern char SceneTitle_StateEntry[];
extern char SceneTitle_StateNewGame[];
extern char SceneTitle_StateOpening[];
extern char SceneTitle_StateOpeningFade[];
extern char SceneTitle_StateStart[];
extern char SceneTitle_StateTitle[];
extern char SceneTitle_dtor[];
extern char Scene_OnSoftReset[];
extern char Scene_Update[];
extern char Scene_dtor[];
extern char ScreenBlend_Draw[];
extern char ScreenBlend_SetParams[];
extern char ScreenBlend_Start[];
extern char ScreenBlend_Update[];
extern char ScreenBlend_dtor[];
extern char Shadow_Draw[];
extern char Shadow_dtor[];
extern char Sinit_Creature[];
extern char Sinit_Debilitas[];
extern char Sinit_Debilitas2[];
extern char Sinit_Debilitas3[];
extern char Sinit_Debilitas3_2[];
extern char Sinit_FillTables[];
extern char Sinit_Flags[];
extern char Sinit_Game[];
extern char Sinit_Iostreams2[];
extern char Sinit_Iostreams3[];
extern char Sinit_Lorenzo[];
extern char Sinit_Lorenzo_2[];
extern char Sinit_Pursuer[];
extern char Sinit_Renderer[];
extern char Sinit_Riccardo[];
extern char Sinit_Room02[];
extern char Sinit_Room03[];
extern char Sinit_Room04[];
extern char Sinit_Room06[];
extern char Sinit_Room08[];
extern char Sinit_Room09[];
extern char Sinit_Room0A[];
extern char Sinit_Room0B[];
extern char Sinit_Room0C[];
extern char Sinit_Room0F[];
extern char Sinit_Room10[];
extern char Sinit_Room102[];
extern char Sinit_Room103[];
extern char Sinit_Room104[];
extern char Sinit_Room105[];
extern char Sinit_Room106[];
extern char Sinit_Room107[];
extern char Sinit_Room108[];
extern char Sinit_Room109[];
extern char Sinit_Room10A[];
extern char Sinit_Room10B[];
extern char Sinit_Room12[];
extern char Sinit_Room13[];
extern char Sinit_Room14[];
extern char Sinit_Room15[];
extern char Sinit_Room18[];
extern char Sinit_Room19[];
extern char Sinit_Room1A[];
extern char Sinit_Room1B[];
extern char Sinit_Room1C[];
extern char Sinit_Room1D[];
extern char Sinit_Room1F[];
extern char Sinit_Room20[];
extern char Sinit_Room21[];
extern char Sinit_Room23[];
extern char Sinit_Room24[];
extern char Sinit_Room25[];
extern char Sinit_Room26[];
extern char Sinit_Room27[];
extern char Sinit_Room28[];
extern char Sinit_Room29[];
extern char Sinit_Room2A[];
extern char Sinit_Room2B[];
extern char Sinit_Room2D[];
extern char Sinit_Room2E[];
extern char Sinit_Room2F[];
extern char Sinit_Room30[];
extern char Sinit_Room31[];
extern char Sinit_Room32[];
extern char Sinit_Room37[];
extern char Sinit_Room40[];
extern char Sinit_Room43[];
extern char Sinit_Room45[];
extern char Sinit_Room46[];
extern char Sinit_Room48[];
extern char Sinit_Room49[];
extern char Sinit_Room4B[];
extern char Sinit_Room4C[];
extern char Sinit_Room4E[];
extern char Sinit_Room50[];
extern char Sinit_Room51[];
extern char Sinit_Room52[];
extern char Sinit_Room54[];
extern char Sinit_Room55[];
extern char Sinit_Room56[];
extern char Sinit_Room59[];
extern char Sinit_Room5A[];
extern char Sinit_Room5C[];
extern char Sinit_Room5D[];
extern char Sinit_Room60[];
extern char Sinit_Room61[];
extern char Sinit_Room62[];
extern char Sinit_Room63[];
extern char Sinit_Room66[];
extern char Sinit_Room69[];
extern char Sinit_Room6A[];
extern char Sinit_Room6C[];
extern char Sinit_Room6D[];
extern char Sinit_Room6F[];
extern char Sinit_Room80[];
extern char Sinit_Room84[];
extern char Sinit_Room85[];
extern char Sinit_Room86[];
extern char Sinit_Room87[];
extern char Sinit_Room88[];
extern char Sinit_Room8C[];
extern char Sinit_Room8D[];
extern char Sinit_Room8E[];
extern char Sinit_Room8F[];
extern char Sinit_Room91[];
extern char Sinit_Room92[];
extern char Sinit_Room93[];
extern char Sinit_Room94[];
extern char Sinit_Room95[];
extern char Sinit_Room96[];
extern char Sinit_Room97[];
extern char Sinit_Room98[];
extern char Sinit_Room99[];
extern char Sinit_Room9A[];
extern char Sinit_Room9B[];
extern char Sinit_RoomC0[];
extern char Sinit_RoomC1[];
extern char Sinit_RoomC2[];
extern char Sinit_RoomC3[];
extern char Sinit_RoomC5[];
extern char Sinit_RoomC7[];
extern char Sinit_RoomC8[];
extern char Sinit_RoomD0[];
extern char Sinit_RoomD1[];
extern char Sinit_RoomD2[];
extern char Sinit_RoomD3[];
extern char Sinit_RoomD4[];
extern char Sinit_RoomD5[];
extern char Sinit_RoomD6[];
extern char Sinit_RoomD7[];
extern char Sinit_RoomD8[];
extern char Sinit_RoomD9[];
extern char Sinit_RoomE0[];
extern char Sinit_RoomE1[];
extern char Sinit_RoomE2[];
extern char Sinit_RoomE3[];
extern char Sinit_RoomE4[];
extern char Sinit_RoomE5[];
extern char Sinit_RoomE6[];
extern char Sinit_RoomE7[];
extern char Sinit_RoomE8[];
extern char Sinit_RoomE9[];
extern char Sinit_SceneGameMembers[];
extern char SinkingSprite_Draw[];
extern char SinkingSprite_SetParams[];
extern char SinkingSprite_Start[];
extern char SinkingSprite_Update[];
extern char SinkingSprite_dtor[];
extern char SlotMachine_Noop14[];
extern char SlotMachine_Start[];
extern char SlotMachine_StateChoose[];
extern char SlotMachine_StateNothing[];
extern char SlotMachine_StateRoll[];
extern char SlotMachine_Update[];
extern char SlotMachine_dtor[];
extern char SmokePuffs_Draw[];
extern char SmokePuffs_SetParams[];
extern char SmokePuffs_Start[];
extern char SmokePuffs_Update[];
extern char SmokePuffs_dtor[];
extern char SmokeTrail_Draw[];
extern char SmokeTrail_SetParams[];
extern char SmokeTrail_Start[];
extern char SmokeTrail_Update[];
extern char SmokeTrail_dtor[];
extern char SndDriverBase_dtor[];
extern char SndDriver_BankFileLoaded[];
extern char SndDriver_BankFileLoaded_thunk[];
extern char SndDriver_BankLoaded[];
extern char SndDriver_BankLoaded_thunk[];
extern char SndDriver_BankLoading[];
extern char SndDriver_BankLoading_thunk[];
extern char SndDriver_Block3D[];
extern char SndDriver_Block3D_thunk[];
extern char SndDriver_CancelFile[];
extern char SndDriver_CancelFile_thunk[];
extern char SndDriver_DropFile[];
extern char SndDriver_DropFile_thunk[];
extern char SndDriver_DropFiles[];
extern char SndDriver_DropFiles_thunk[];
extern char SndDriver_GetVolume[];
extern char SndDriver_GetVolume_thunk[];
extern char SndDriver_Header[];
extern char SndDriver_Header_thunk[];
extern char SndDriver_LoadPart[];
extern char SndDriver_LoadPart_thunk[];
extern char SndDriver_OutputMode[];
extern char SndDriver_OutputMode_thunk[];
extern char SndDriver_PlacedState[];
extern char SndDriver_PlacedState_thunk[];
extern char SndDriver_Play[];
extern char SndDriver_PlayPlaced[];
extern char SndDriver_PlayPlaced_thunk[];
extern char SndDriver_PlayPositioned[];
extern char SndDriver_PlayPositioned_thunk[];
extern char SndDriver_Play_thunk[];
extern char SndDriver_QueueSamples[];
extern char SndDriver_QueueSamples_thunk[];
extern char SndDriver_Register[];
extern char SndDriver_Register_thunk[];
extern char SndDriver_SamplesNow[];
extern char SndDriver_SamplesNow_thunk[];
extern char SndDriver_SendMaster[];
extern char SndDriver_SendMaster_thunk[];
extern char SndDriver_SeqParam1[];
extern char SndDriver_SeqParam1B[];
extern char SndDriver_SeqParam1B_thunk[];
extern char SndDriver_SeqParam1_thunk[];
extern char SndDriver_SeqParam2[];
extern char SndDriver_SeqParam2B[];
extern char SndDriver_SeqParam2B_thunk[];
extern char SndDriver_SeqParam2_thunk[];
extern char SndDriver_SeqParamPtr[];
extern char SndDriver_SeqParamPtr_thunk[];
extern char SndDriver_SeqPlay[];
extern char SndDriver_SeqPlay_thunk[];
extern char SndDriver_SeqRestart[];
extern char SndDriver_SeqRestart_thunk[];
extern char SndDriver_SeqStop[];
extern char SndDriver_SeqStop_thunk[];
extern char SndDriver_SeqTempo[];
extern char SndDriver_SeqTempo_thunk[];
extern char SndDriver_SeqVolume[];
extern char SndDriver_SeqVolume_thunk[];
extern char SndDriver_Sequence[];
extern char SndDriver_Sequence_thunk[];
extern char SndDriver_SetMaster[];
extern char SndDriver_SetMaster_thunk[];
extern char SndDriver_SetOutputMode[];
extern char SndDriver_SetOutputMode_thunk[];
extern char SndDriver_SetSeqTempo[];
extern char SndDriver_SetSeqTempo_thunk[];
extern char SndDriver_SetSeqVolume[];
extern char SndDriver_SetSeqVolume_thunk[];
extern char SndDriver_SetSound[];
extern char SndDriver_SetSound_thunk[];
extern char SndDriver_SetVolume[];
extern char SndDriver_SetVolume_thunk[];
extern char SndDriver_StopAll[];
extern char SndDriver_StopAll_thunk[];
extern char SndDriver_StopMasks[];
extern char SndDriver_StopMasks_thunk[];
extern char SndDriver_StopPlaced[];
extern char SndDriver_StopPlacedButBanks[];
extern char SndDriver_StopPlacedButBanks_thunk[];
extern char SndDriver_StopPlaced_thunk[];
extern char SndDriver_StopSound[];
extern char SndDriver_StopSound_thunk[];
extern char SndDriver_StopVoices[];
extern char SndDriver_StopVoices_thunk[];
extern char SndDriver_Table[];
extern char SndDriver_Table_thunk[];
extern char SndDriver_TransferRunning[];
extern char SndDriver_TransferRunning_thunk[];
extern char SndDriver_Unload[];
extern char SndDriver_Unload_thunk[];
extern char SndDriver_dtor[];
extern char SndDriver_dtor_thunk[];
extern char SparkSpray_Draw[];
extern char SparkSpray_SetParams[];
extern char SparkSpray_Start[];
extern char SparkSpray_Update[];
extern char SparkSpray_dtor[];
extern char Spark_Draw[];
extern char Spark_SetParams[];
extern char Spark_Start[];
extern char Spark_Update[];
extern char Spark_dtor[];
extern char SpeckSwarm_Draw[];
extern char SpeckSwarm_SetParams[];
extern char SpeckSwarm_Start[];
extern char SpeckSwarm_Update[];
extern char SpeckSwarm_dtor[];
extern char SpiralSmoke_Draw[];
extern char SpiralSmoke_Start[];
extern char SpiralSmoke_Update[];
extern char SpiralSmoke_dtor[];
extern char SplashRing_Draw[];
extern char SplashRing_SetParams[];
extern char SplashRing_Start[];
extern char SplashRing_Update[];
extern char SplashRing_dtor[];
extern char Splash_Draw[];
extern char Splash_SetParams[];
extern char Splash_Start[];
extern char Splash_Update[];
extern char Splash_dtor[];
extern char SpringPartBase_AdjustBone[];
extern char SpringPart_Noop10[];
extern char SpringPart_NoopC[];
extern char SpringPart_Reset[];
extern char SpriteBurst_Draw[];
extern char SpriteBurst_SetParams[];
extern char SpriteBurst_Start[];
extern char SpriteBurst_Update[];
extern char SpriteBurst_dtor[];
extern char SprungPoint_AdjustBone[];
extern char SprungPoint_Frame[];
extern char StrandSplash_Draw[];
extern char StrandSplash_SetParams[];
extern char StrandSplash_Start[];
extern char StrandSplash_Update[];
extern char StrandSplash_dtor[];
extern char StrikeMark_Draw[];
extern char StrikeMark_SetParams[];
extern char StrikeMark_Start[];
extern char StrikeMark_Update[];
extern char StrikeMark_dtor[];
extern char SubPool_dtor[];
extern char SubScreenBase_dtor[];
extern char SubScreen_AddFile[];
extern char SubScreen_Byte97A8F[];
extern char SubScreen_Costume[];
extern char SubScreen_CostumeModel[];
extern char SubScreen_DrawClosing[];
extern char SubScreen_DrawFadeBack[];
extern char SubScreen_DrawFadeFromBlack[];
extern char SubScreen_DrawFadeIn[];
extern char SubScreen_DrawFadeOut[];
extern char SubScreen_DrawLoadWait[];
extern char SubScreen_DrawOpening[];
extern char SubScreen_DrawRun[];
extern char SubScreen_DrawTab[];
extern char SubScreen_Open[];
extern char SubScreen_Part97980[];
extern char SubScreen_SetBit[];
extern char SubScreen_SetLayout[];
extern char SubScreen_StateEntries[];
extern char SubScreen_StateEntryQuestion[];
extern char SubScreen_StateEquipAsk[];
extern char SubScreen_StateExtras[];
extern char SubScreen_StateExtrasMenu[];
extern char SubScreen_StateFiles[];
extern char SubScreen_StateItemActions[];
extern char SubScreen_StateItemQuestion[];
extern char SubScreen_StateItems[];
extern char SubScreen_StateLoad[];
extern char SubScreen_StateMap[];
extern char SubScreen_StateMovies[];
extern char SubScreen_StatePageObject[];
extern char SubScreen_StatePageOpen[];
extern char SubScreen_StateQuestion[];
extern char SubScreen_StateRun[];
extern char SubScreen_StateSave[];
extern char SubScreen_StateThrowAsk[];
extern char SubScreen_StateWordMake[];
extern char SubScreen_StateWordPlate[];
extern char SubScreen_TabCommand[];
extern char SubScreen_TakeOver[];
extern char SubScreen_TestBit[];
extern char SubScreen_UseEquipped[];
extern char SubScreen_dtor[];
extern char SwayPointA_Pose[];
extern char SwayPointA_Step[];
extern char SwayPointB_Pose[];
extern char SwayPointB_Step[];
extern char SynthBase_Destroy[];
extern char SynthBase_Noop14[];
extern char SynthBase_Reset[];
extern char SynthBase_Update[];
extern char SynthPot_Noop14[];
extern char SynthPot_Start[];
extern char SynthPot_StateDebug[];
extern char SynthPot_Update[];
extern char SynthPot_dtor[];
extern char SystemBase_dtor[];
extern char System_EndFrame[];
extern char System_FrameNoWait[];
extern char System_Init[];
extern char System_Shutdown[];
extern char System_dtor[];
extern char Task_StateChild[];
extern char Task_StateChoice[];
extern char Task_StateGlyphDelay[];
extern char Task_StateIdle[];
extern char Task_StateNextPage[];
extern char Task_StatePause[];
extern char Task_StateType[];
extern char Task_StateWaitButton[];
extern char Task_StateWaitFrames[];
extern char TexCache_AddGroup[];
extern char TexCache_Entry[];
extern char TexCache_ForgetLayers[];
extern char TexCache_Layer[];
extern char TexCache_RemoveGroup[];
extern char TextObj_dtor[];
extern char Thing01_Draw[];
extern char Thing01_Flying[];
extern char Thing01_Frame[];
extern char Thing01_LandTri[];
extern char Thing01_MotionState[];
extern char Thing01_PutDown[];
extern char Thing01_Setup[];
extern char Thing01_StateDropping[];
extern char Thing01_dtor[];
extern char Thing02_Draw[];
extern char Thing02_Frame[];
extern char Thing02_Setup[];
extern char Thing02_StateDropping[];
extern char Thing02_StateRolling[];
extern char Thing02_dtor[];
extern char Thing03_Draw[];
extern char Thing03_Flying[];
extern char Thing03_Frame[];
extern char Thing03_LandTri[];
extern char Thing03_MotionState[];
extern char Thing03_PutDown[];
extern char Thing03_Setup[];
extern char Thing03_StateDropping[];
extern char Thing03_StateRolling[];
extern char Thing03_dtor[];
extern char Thing04_Draw[];
extern char Thing04_Flying[];
extern char Thing04_Frame[];
extern char Thing04_LandTri[];
extern char Thing04_MotionState[];
extern char Thing04_PutDown[];
extern char Thing04_Setup[];
extern char Thing04_StateDropping[];
extern char Thing04_dtor[];
extern char Thing05_Draw[];
extern char Thing05_Frame[];
extern char Thing05_Setup[];
extern char Thing05_dtor[];
extern char Thing06_Draw[];
extern char Thing06_Flying[];
extern char Thing06_Frame[];
extern char Thing06_LandTri[];
extern char Thing06_MotionState[];
extern char Thing06_PutDown[];
extern char Thing06_Setup[];
extern char Thing06_StateDropping[];
extern char Thing06_dtor[];
extern char Thing07_Draw[];
extern char Thing07_Frame[];
extern char Thing07_Setup[];
extern char Thing07_dtor[];
extern char Thing08_Draw[];
extern char Thing08_Frame[];
extern char Thing08_Setup[];
extern char Thing08_dtor[];
extern char Thing09_Draw[];
extern char Thing09_Flying[];
extern char Thing09_Frame[];
extern char Thing09_MotionState[];
extern char Thing09_Setup[];
extern char Thing09_StateFalling[];
extern char Thing09_StateRolling[];
extern char Thing09_dtor[];
extern char Thing10_Draw[];
extern char Thing10_Flying[];
extern char Thing10_Frame[];
extern char Thing10_MotionState[];
extern char Thing10_Setup[];
extern char Thing10_StateFalling[];
extern char Thing10_StateRolling[];
extern char Thing10_dtor[];
extern char ThingBurst_Draw[];
extern char ThingBurst_SetParams[];
extern char ThingBurst_Start[];
extern char ThingBurst_Update[];
extern char ThingBurst_dtor[];
extern char ThingPuff_Draw[];
extern char ThingPuff_SetParams[];
extern char ThingPuff_Start[];
extern char ThingPuff_Update[];
extern char ThingPuff_dtor[];
extern char ThingShared_Draw[];
extern char ThingShared_Flying[];
extern char ThingShared_Frame[];
extern char ThingShared_LandTri[];
extern char ThingShared_MotionState[];
extern char ThingShared_PutDown[];
extern char ThingShared_Setup[];
extern char ThingShared_dtor[];
extern char Thing_Draw[];
extern char Thing_Frame[];
extern char Thing_LandTri[];
extern char Thing_MotionState[];
extern char Thing_PutAt[];
extern char Thing_PutDown[];
extern char Thing_Setup[];
extern char Thing_dtor[];
extern char TintDrawer_dtor[];
extern char TintStalker_ActionOffsets[];
extern char TintStalker_AttackAnimA[];
extern char TintStalker_AttackAnimB[];
extern char TintStalker_AttackTable[];
extern char TintStalker_BreakDoor[];
extern char TintStalker_Disable[];
extern char TintStalker_DoorDone[];
extern char TintStalker_DoorOffset[];
extern char TintStalker_Draw[];
extern char TintStalker_Enable[];
extern char TintStalker_EndingDoor[];
extern char TintStalker_FrightAttack[];
extern char TintStalker_FrightSeen[];
extern char TintStalker_MotionFiles[];
extern char TintStalker_ReachHewie[];
extern char TintStalker_Reset[];
extern char TintStalker_SetDelay[];
extern char TintStalker_Setup[];
extern char TintStalker_SlowWalkAnim[];
extern char TintStalker_Table[];
extern char TintStalker_Update[];
extern char TintStalker_dtor[];
extern char Tint_Draw[];
extern char Tint_SetParams[];
extern char Tint_Start[];
extern char Tint_Update[];
extern char TurningModel_Draw[];
extern char TurningModel_Start[];
extern char TurningModel_Update[];
extern char TurningModel_dtor[];
extern char TvScreenA_Draw[];
extern char TvScreenA_SetParams[];
extern char TvScreenA_Start[];
extern char TvScreenA_Update[];
extern char TvScreenA_dtor[];
extern char TvScreenB_Draw[];
extern char TvScreenB_SetParams[];
extern char TvScreenB_Start[];
extern char TvScreenB_Update[];
extern char TvScreenB_dtor[];
extern char VramBase_dtor[];
extern char Vram_Alloc[];
extern char Vram_AllocAt[];
extern char Vram_AllocTexture[];
extern char Vram_ClutAddr[];
extern char Vram_EntryTex0[];
extern char Vram_EntryTex0Csa[];
extern char Vram_EntryTex0Indexed[];
extern char Vram_EntryTex2[];
extern char Vram_Free[];
extern char Vram_FreeAll[];
extern char Vram_Height[];
extern char Vram_Init[];
extern char Vram_KeepResident[];
extern char Vram_LoadTex[];
extern char Vram_LoadTexUpper[];
extern char Vram_Reset[];
extern char Vram_Tex[];
extern char Vram_Tex0Csa[];
extern char Vram_Tex0Indexed[];
extern char Vram_Tex0Load[];
extern char Vram_Tex2[];
extern char Vram_TexAddr[];
extern char Vram_TexUpper[];
extern char Vram_Upload[];
extern char Vram_Width[];
extern char Vram_dtor[];
extern char WallShadow_Draw[];
extern char WallShadow_SetParams[];
extern char WallShadow_Start[];
extern char WallShadow_Update[];
extern char WallShadow_dtor[];
extern char WindHangPoint_AdjustBone[];
extern char WindHangPoint_Frame[];
extern char WindowFlash_Draw[];
extern char WindowFlash_SetParams[];
extern char WindowFlash_Start[];
extern char WindowFlash_Update[];
extern char WindowFlash_dtor[];
extern char WispColumn_Draw[];
extern char WispColumn_SetParams[];
extern char WispColumn_Start[];
extern char WispColumn_Update[];
extern char WispColumn_dtor[];
extern char Wisps_Draw[];
extern char Wisps_SetParams[];
extern char Wisps_Start[];
extern char Wisps_Update[];
extern char Wisps_dtor[];
extern char func_001004F0[];
extern char func_001005F0[];
extern char func_00101420[];
extern char func_00102730[];
extern char func_001027C0[];
extern char func_001028A0[];
extern char func_001028B0[];
extern char func_001029B0[];
extern char func_00102CC0[];
extern char func_00103030[];
extern char func_001031B0[];
extern char func_00103250[];
extern char func_001035A0[];
extern char func_00103720[];
extern char func_001037C0[];
extern char func_00103860[];
extern char func_001038C0[];
extern char func_00103940[];
extern char func_001039A0[];
extern char func_00103A20[];
extern char func_00103A30[];
extern char func_00103A40[];
extern char func_00103A50[];
extern char func_00103A60[];
extern char func_00103A70[];
extern char func_00103A80[];
extern char func_00103A90[];
extern char func_00103AF0[];
extern char func_00103B00[];
extern char func_00103B10[];
extern char func_00103C40[];
extern char func_00103D70[];
extern char func_00103D80[];
extern char func_00103D90[];
extern char func_00103DB0[];
extern char func_00103DC0[];
extern char func_00103FA0[];
extern char func_00103FB0[];
extern char func_00104010[];
extern char func_00104020[];
extern char func_00104200[];
extern char func_00104210[];
extern char func_00104220[];
extern char func_001044B0[];
extern char func_00104AC0[];
extern char func_00104BA0[];
extern char func_00104BD0[];
extern char func_00104C90[];
extern char func_00104E50[];
extern char func_00104E60[];
extern char func_001050E0[];
extern char func_00105550[];
extern char func_00105620[];
extern char func_001057E0[];
extern char func_001059E0[];
extern char func_00105B30[];
extern char func_00105C10[];
extern char func_00105C40[];
extern char func_00105D00[];
extern char func_00105EC0[];
extern char func_00105ED0[];
extern char func_00106160[];
extern char func_00106640[];
extern char func_00106710[];
extern char func_001068D0[];
extern char func_00106AE0[];
extern char func_001079A0[];
extern char func_001079B0[];
extern char func_001079C0[];
extern char func_001080B0[];
extern char func_001086C0[];
extern char func_001086D0[];
extern char func_00108720[];
extern char func_00108780[];
extern char func_001087E0[];
extern char func_00108880[];
extern char func_00108920[];
extern char func_00108960[];
extern char func_00108DD0[];
extern char func_00108EE0[];
extern char func_00108F30[];
extern char func_00108FB0[];
extern char func_00109040[];
extern char func_001090D0[];
extern char func_00109100[];
extern char func_00109170[];
extern char func_001091A0[];
extern char func_00109210[];
extern char func_00109240[];
extern char func_00109270[];
extern char func_00109EC0[];
extern char func_00109F70[];
extern char func_0010AF90[];
extern char func_0010B100[];
extern char func_0010B110[];
extern char func_0010B120[];
extern char func_0010B130[];
extern char func_0010B350[];
extern char func_0010B3E0[];
extern char func_0010B400[];
extern char func_0010B440[];
extern char func_0010B460[];
extern char func_0010B4A0[];
extern char func_0010B4B0[];
extern char func_0010B4E0[];
extern char func_0010B4F0[];
extern char func_0010BAB0[];
extern char func_00179AB0[];
extern char func_001BA970[];
extern char func_001BAB80[];
extern char func_001BB780[];
extern char func_001BDF80[];
extern char func_001BE790[];
extern char func_001BED00[];
extern char func_001BF860[];
extern char func_001BF870[];
extern char func_0020C1D0[];
extern char func_0020C1E0[];
extern char func_0020C1F0[];
extern char func_0020C200[];
extern char func_0020C210[];
extern char func_0020C220[];
extern char func_0020C230[];
extern char func_0020C240[];
extern char func_0020C250[];
extern char func_0020C260[];
extern char func_0020C270[];
extern char func_0020C280[];
extern char func_0020C290[];
extern char func_0020C2A0[];
extern char func_0020C2B0[];
extern char func_0020C2C0[];
extern char func_0020C2D0[];
extern char func_0020C2E0[];
extern char func_0020C2F0[];
extern char func_0020C300[];
extern char func_0020C310[];
extern char func_0020C320[];
extern char func_0020C330[];
extern char func_0020C340[];
extern char func_0020C350[];
extern char func_0020C360[];
extern char func_0020C370[];
extern char func_0020C380[];
extern char func_0020C390[];
extern char func_00225680[];
extern char func_00225690[];
extern char func_002256A0[];
extern char func_002256B0[];
extern char func_002256C0[];
extern char func_002256D0[];
extern char func_002256E0[];
extern char func_002256F0[];
extern char func_00225700[];
extern char func_00225710[];
extern char func_00225720[];
extern char func_00225730[];
extern char func_00225740[];
extern char func_00225750[];
extern char func_00225760[];
extern char func_00225770[];
extern char func_00225860[];
extern char func_00225950[];
extern char func_00225C40[];
extern char func_00225F30[];
extern char func_00226220[];
extern char func_00268180[];
extern char func_002685F0[];
extern char func_002BB3E0[];
extern char func_002C6650[];
extern char func_00308FE0[];
extern char func_0030D9E0[];
extern char func_00311AD0[];
extern char func_00315C60[];
extern char func_00316BE0[];
extern char func_0031D820[];
extern char func_0031E6F0[];
extern char func_00320D60[];
extern char func_00321770[];
extern char func_0032C370[];
extern char func_0032CAF0[];
extern char func_0032CF30[];
extern char func_00330770[];
extern char func_00330780[];
extern char func_00337CE0[];
extern char func_00338AE0[];
extern char func_00339B40[];
extern char func_0033ACD0[];
extern char func_0033B050[];
extern char func_0033B320[];
extern char func_0033D7F0[];
extern char func_00345230[];
extern char func_00349FE0[];
extern char func_00352050[];
extern char func_003543B0[];
extern char func_0035B110[];
extern char func_0035B310[];
extern char func_003854B0[];
extern char func_003A19B0[];
extern char func_003A19C0[];
extern char func_003A19D0[];
extern char func_003A19E0[];
extern char func_003A19F0[];
extern char func_003A1A00[];
extern char func_003A1A10[];
extern char func_003A1A20[];
extern char func_003A1A30[];
extern char func_003A1A40[];
extern char func_003A1A50[];
extern char func_003A1A60[];
extern char func_003A1A70[];
extern char func_003A1A80[];
extern char func_003A1A90[];
extern char func_003A1AA0[];
extern char func_003A1AB0[];
extern char func_003A1AC0[];
extern char func_003A1AD0[];
extern char func_003A1AE0[];
extern char func_003A1AF0[];
extern char func_003A1B00[];
extern char func_003A1B10[];
extern char func_003A1B20[];
extern char func_003A1B30[];
extern char func_003A1B40[];
extern char func_003A1B50[];
extern char func_003A1B60[];
extern char func_003A1B70[];

/* 0x003AF290: Debilitas_StartWander_ptmf, D_003AF298, Debilitas_StartTurnToFiona_ptmf, D_003AF2A4, D_003AF2A8, Debilitas_StartLunge_ptmf ... */
void *g1_003AF290[16] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas_StartWander, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas_StartTurnToFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas_StartLunge, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas_StartGrab, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Debilitas_StartWander_ptmf", "g1_003AF290", "0x0") G1_ALIAS("D_003AF298", "g1_003AF290", "0x8") G1_ALIAS("Debilitas_StartTurnToFiona_ptmf", "g1_003AF290", "0x10") G1_ALIAS("D_003AF2A4", "g1_003AF290", "0x14") G1_ALIAS("D_003AF2A8", "g1_003AF290", "0x18") G1_ALIAS("Debilitas_StartLunge_ptmf", "g1_003AF290", "0x20") G1_ALIAS("D_003AF2B4", "g1_003AF290", "0x24") G1_ALIAS("D_003AF2B8", "g1_003AF290", "0x28") G1_ALIAS("Debilitas_StartGrab_ptmf", "g1_003AF290", "0x30") G1_ALIAS("D_003AF2C4", "g1_003AF290", "0x34") G1_ALIAS("D_003AF2C8", "g1_003AF290", "0x38"));

/* 0x003AFEF0: Debilitas_Behaviour_ptmf, D_003AFEF8 */
void *g1_003AFEF0[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas_Behaviour, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Debilitas_Behaviour_ptmf", "g1_003AFEF0", "0x0") G1_ALIAS("D_003AFEF8", "g1_003AFEF0", "0x8"));

/* 0x003AFF20: Debilitas_Chase_ptmf, D_003AFF28 */
void *g1_003AFF20[4] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas_Chase, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Debilitas_Chase_ptmf", "g1_003AFF20", "0x0") G1_ALIAS("D_003AFF28", "g1_003AFF20", "0x8"));

/* 0x003AFFA0: Debilitas_StateStartWalk_ptmf, D_003AFFA8, Debilitas_StateLookWalk_ptmf, D_003AFFB8, Debilitas_StateTurnToFiona_ptmf, D_003AFFC8 ... */
void *g1_003AFFA0[36] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas_StateStartWalk, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas_StateLookWalk, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas_StateTurnToFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas_StateBlow, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas_StateStun, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas_StateStun, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas_StateLunge, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_AttackNextStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas_StateGrab, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Debilitas_StateStartWalk_ptmf", "g1_003AFFA0", "0x0") G1_ALIAS("D_003AFFA8", "g1_003AFFA0", "0x8") G1_ALIAS("Debilitas_StateLookWalk_ptmf", "g1_003AFFA0", "0x10") G1_ALIAS("D_003AFFB8", "g1_003AFFA0", "0x18") G1_ALIAS("Debilitas_StateTurnToFiona_ptmf", "g1_003AFFA0", "0x20") G1_ALIAS("D_003AFFC8", "g1_003AFFA0", "0x28") G1_ALIAS("Debilitas_StateBlow_ptmf", "g1_003AFFA0", "0x30") G1_ALIAS("D_003AFFD8", "g1_003AFFA0", "0x38") G1_ALIAS("Debilitas_StateStun_ptmf", "g1_003AFFA0", "0x40") G1_ALIAS("D_003AFFE8", "g1_003AFFA0", "0x48") G1_ALIAS("Debilitas_StateStun_ptmf2", "g1_003AFFA0", "0x50") G1_ALIAS("D_003AFFF8", "g1_003AFFA0", "0x58") G1_ALIAS("Debilitas_StateLunge_ptmf", "g1_003AFFA0", "0x60") G1_ALIAS("D_003B0008", "g1_003AFFA0", "0x68") G1_ALIAS("Pursuer_AttackNextStep_ptmf", "g1_003AFFA0", "0x70") G1_ALIAS("D_003B0018", "g1_003AFFA0", "0x78") G1_ALIAS("Debilitas_StateGrab_ptmf", "g1_003AFFA0", "0x80") G1_ALIAS("D_003B0028", "g1_003AFFA0", "0x88"));

/* 0x003B0070: D_003B0070, D_003B0078, D_003B0080, D_003B0088, D_003B0090, D_003B0098 ... */
void *g1_003B0070[148] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_StateStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_StateAttractStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_StateTitle, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_SeqLoad, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_StateAttract, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_StateAttractFade, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_StateTitle, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_SeqLoad, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_StateDemo, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_StateDemoFade, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_StateTitle, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_SeqLoad, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_SeqPressStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_SeqFadeIn, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_SeqPressStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_SeqToMenu, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_SeqLeave, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_SeqMenu, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_SeqMenuToTitle, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_SeqLoadGame, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_SeqOptions, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_SeqLeave, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_SeqLeave, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_SeqLeave, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_SeqMenu, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_SeqMenu, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_StateAttractStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_StateDemoStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_StateNewGame, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_StateNewGame, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_SeqPressStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_StateOpening, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneTitle_StateOpeningFade, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StatePending, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateTense, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateWary, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateCalm, (void *)0x00000000u,
};
__asm__(G1_ALIAS("D_003B0070", "g1_003B0070", "0x0") G1_ALIAS("D_003B0078", "g1_003B0070", "0x8") G1_ALIAS("D_003B0080", "g1_003B0070", "0x10") G1_ALIAS("D_003B0088", "g1_003B0070", "0x18") G1_ALIAS("D_003B0090", "g1_003B0070", "0x20") G1_ALIAS("D_003B0098", "g1_003B0070", "0x28") G1_ALIAS("D_003B00A0", "g1_003B0070", "0x30") G1_ALIAS("D_003B00A8", "g1_003B0070", "0x38") G1_ALIAS("D_003B00B0", "g1_003B0070", "0x40") G1_ALIAS("D_003B00B8", "g1_003B0070", "0x48") G1_ALIAS("D_003B00C0", "g1_003B0070", "0x50") G1_ALIAS("D_003B00C8", "g1_003B0070", "0x58") G1_ALIAS("D_003B00D0", "g1_003B0070", "0x60") G1_ALIAS("D_003B00D8", "g1_003B0070", "0x68") G1_ALIAS("D_003B00E0", "g1_003B0070", "0x70") G1_ALIAS("D_003B00E8", "g1_003B0070", "0x78") G1_ALIAS("D_003B00F0", "g1_003B0070", "0x80") G1_ALIAS("D_003B00F8", "g1_003B0070", "0x88") G1_ALIAS("D_003B0100", "g1_003B0070", "0x90") G1_ALIAS("D_003B0108", "g1_003B0070", "0x98") G1_ALIAS("D_003B0110", "g1_003B0070", "0xA0") G1_ALIAS("D_003B0118", "g1_003B0070", "0xA8") G1_ALIAS("D_003B0120", "g1_003B0070", "0xB0") G1_ALIAS("D_003B0128", "g1_003B0070", "0xB8") G1_ALIAS("D_003B0130", "g1_003B0070", "0xC0") G1_ALIAS("D_003B0138", "g1_003B0070", "0xC8") G1_ALIAS("D_003B0140", "g1_003B0070", "0xD0") G1_ALIAS("D_003B0148", "g1_003B0070", "0xD8") G1_ALIAS("D_003B0150", "g1_003B0070", "0xE0") G1_ALIAS("D_003B0158", "g1_003B0070", "0xE8") G1_ALIAS("D_003B0160", "g1_003B0070", "0xF0") G1_ALIAS("D_003B0168", "g1_003B0070", "0xF8") G1_ALIAS("D_003B0170", "g1_003B0070", "0x100") G1_ALIAS("D_003B0178", "g1_003B0070", "0x108") G1_ALIAS("D_003B0180", "g1_003B0070", "0x110") G1_ALIAS("D_003B0188", "g1_003B0070", "0x118") G1_ALIAS("D_003B0190", "g1_003B0070", "0x120") G1_ALIAS("D_003B0198", "g1_003B0070", "0x128") G1_ALIAS("D_003B01A0", "g1_003B0070", "0x130") G1_ALIAS("D_003B01A8", "g1_003B0070", "0x138") G1_ALIAS("D_003B01B0", "g1_003B0070", "0x140") G1_ALIAS("D_003B01B8", "g1_003B0070", "0x148") G1_ALIAS("D_003B01C0", "g1_003B0070", "0x150") G1_ALIAS("D_003B01C8", "g1_003B0070", "0x158") G1_ALIAS("D_003B01D0", "g1_003B0070", "0x160") G1_ALIAS("D_003B01D8", "g1_003B0070", "0x168") G1_ALIAS("D_003B01E0", "g1_003B0070", "0x170") G1_ALIAS("D_003B01E8", "g1_003B0070", "0x178") G1_ALIAS("D_003B01F0", "g1_003B0070", "0x180") G1_ALIAS("D_003B01F8", "g1_003B0070", "0x188") G1_ALIAS("D_003B0200", "g1_003B0070", "0x190") G1_ALIAS("D_003B0208", "g1_003B0070", "0x198") G1_ALIAS("D_003B0210", "g1_003B0070", "0x1A0") G1_ALIAS("D_003B0218", "g1_003B0070", "0x1A8") G1_ALIAS("D_003B0220", "g1_003B0070", "0x1B0") G1_ALIAS("D_003B0228", "g1_003B0070", "0x1B8") G1_ALIAS("D_003B0230", "g1_003B0070", "0x1C0") G1_ALIAS("D_003B0238", "g1_003B0070", "0x1C8") G1_ALIAS("D_003B0240", "g1_003B0070", "0x1D0") G1_ALIAS("D_003B0248", "g1_003B0070", "0x1D8") G1_ALIAS("SceneTitle_SeqPressStart_ptmf", "g1_003B0070", "0x1E0") G1_ALIAS("D_003B0258", "g1_003B0070", "0x1E8") G1_ALIAS("D_003B0260", "g1_003B0070", "0x1F0") G1_ALIAS("D_003B0268", "g1_003B0070", "0x1F8") G1_ALIAS("D_003B0270", "g1_003B0070", "0x200") G1_ALIAS("D_003B0278", "g1_003B0070", "0x208") G1_ALIAS("Hewie_StatePending_ptmf", "g1_003B0070", "0x210") G1_ALIAS("D_003B0288", "g1_003B0070", "0x218") G1_ALIAS("Hewie_StateTense_ptmf", "g1_003B0070", "0x220") G1_ALIAS("D_003B0298", "g1_003B0070", "0x228") G1_ALIAS("Hewie_StateWary_ptmf", "g1_003B0070", "0x230") G1_ALIAS("D_003B02A8", "g1_003B0070", "0x238") G1_ALIAS("Hewie_StateCalm_ptmf", "g1_003B0070", "0x240") G1_ALIAS("D_003B02B8", "g1_003B0070", "0x248"));

/* 0x003B1708: Hewie_StateAnimOver2_ptmf, D_003B170C, D_003B1710, Hewie_StateAnimOver2_ptmf2, D_003B171C, D_003B1720 ... */
void *g1_003B1708[318] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateAnimOver2, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateAnimOver2, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateAnimOver2, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1740, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateComeToFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1740, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1770, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateAnimOver3, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateAnimOver2, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateAnimOver2, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateAnimOver2, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateAnimOver2, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateAnimOver2, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateGoesForPursuer, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateGoForPursuer2, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateLeapGrip, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateLeapLands, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateBiteHold, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateClamped, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateClamped, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateHanging, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateClamped, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateHanging, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateClamped, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateJumpArc, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateJumpArc, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State18B0, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateFalling, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State18B0, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateTackle, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateLeapBite, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateLanding, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateLanding, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1920, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateOnEvent20, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateWalkHeading, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateAnimNext, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateAnimNext, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateRush, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1980, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1990, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State19A0, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State19B0, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateBackToNormal, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateBehindFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateByFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StatePlayToEnd, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateGoForPursuer, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateLeapLaunch, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateLeapTakeOff, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateLeapAtPursuer, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateBiteHold, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateSlideToCommand, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1A60, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateSlideToCommand, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1A60, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateSlideToCommand, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1AA0, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1A60, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1A60, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1A60, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1A60, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1AF0, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1B00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateFollowCommand, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1AF0, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1B00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1B40, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1A60, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1B60, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1B00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1A60, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateGuard, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateFlee, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateRunSpot, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StatePursuerNear, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StatePlanPursuer, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateGuard, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StatePlanPursuer, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Hewie_StateAnimOver2_ptmf", "g1_003B1708", "0x0") G1_ALIAS("D_003B170C", "g1_003B1708", "0x4") G1_ALIAS("D_003B1710", "g1_003B1708", "0x8") G1_ALIAS("Hewie_StateAnimOver2_ptmf2", "g1_003B1708", "0x10") G1_ALIAS("D_003B171C", "g1_003B1708", "0x14") G1_ALIAS("D_003B1720", "g1_003B1708", "0x18") G1_ALIAS("Hewie_StateAnimOver2_ptmf3", "g1_003B1708", "0x20") G1_ALIAS("D_003B172C", "g1_003B1708", "0x24") G1_ALIAS("D_003B1730", "g1_003B1708", "0x28") G1_ALIAS("Hewie_State1740_ptmf", "g1_003B1708", "0x30") G1_ALIAS("D_003B173C", "g1_003B1708", "0x34") G1_ALIAS("D_003B1740", "g1_003B1708", "0x38") G1_ALIAS("Hewie_StateComeToFiona_ptmf", "g1_003B1708", "0x40") G1_ALIAS("D_003B174C", "g1_003B1708", "0x44") G1_ALIAS("D_003B1750", "g1_003B1708", "0x48") G1_ALIAS("Hewie_State1740_ptmf2", "g1_003B1708", "0x50") G1_ALIAS("D_003B175C", "g1_003B1708", "0x54") G1_ALIAS("D_003B1760", "g1_003B1708", "0x58") G1_ALIAS("Hewie_State1770_ptmf", "g1_003B1708", "0x60") G1_ALIAS("D_003B176C", "g1_003B1708", "0x64") G1_ALIAS("D_003B1770", "g1_003B1708", "0x68") G1_ALIAS("Hewie_StateAnimOver3_ptmf", "g1_003B1708", "0x70") G1_ALIAS("D_003B177C", "g1_003B1708", "0x74") G1_ALIAS("D_003B1780", "g1_003B1708", "0x78") G1_ALIAS("Hewie_StateAnimOver2_ptmf4", "g1_003B1708", "0x80") G1_ALIAS("D_003B178C", "g1_003B1708", "0x84") G1_ALIAS("D_003B1790", "g1_003B1708", "0x88") G1_ALIAS("Hewie_StateAnimOver2_ptmf5", "g1_003B1708", "0x90") G1_ALIAS("D_003B179C", "g1_003B1708", "0x94") G1_ALIAS("D_003B17A0", "g1_003B1708", "0x98") G1_ALIAS("Hewie_StateAnimOver2_ptmf6", "g1_003B1708", "0xA0") G1_ALIAS("D_003B17AC", "g1_003B1708", "0xA4") G1_ALIAS("D_003B17B0", "g1_003B1708", "0xA8") G1_ALIAS("Hewie_StateAnimOver2_ptmf7", "g1_003B1708", "0xB0") G1_ALIAS("D_003B17BC", "g1_003B1708", "0xB4") G1_ALIAS("D_003B17C0", "g1_003B1708", "0xB8") G1_ALIAS("Hewie_StateAnimOver2_ptmf8", "g1_003B1708", "0xC0") G1_ALIAS("D_003B17CC", "g1_003B1708", "0xC4") G1_ALIAS("D_003B17D0", "g1_003B1708", "0xC8") G1_ALIAS("Hewie_StateGoesForPursuer_ptmf", "g1_003B1708", "0xD0") G1_ALIAS("D_003B17DC", "g1_003B1708", "0xD4") G1_ALIAS("D_003B17E0", "g1_003B1708", "0xD8") G1_ALIAS("Hewie_StateGoForPursuer2_ptmf", "g1_003B1708", "0xE0") G1_ALIAS("D_003B17EC", "g1_003B1708", "0xE4") G1_ALIAS("D_003B17F0", "g1_003B1708", "0xE8") G1_ALIAS("Hewie_StateLeapGrip_ptmf", "g1_003B1708", "0xF0") G1_ALIAS("D_003B17FC", "g1_003B1708", "0xF4") G1_ALIAS("D_003B1800", "g1_003B1708", "0xF8") G1_ALIAS("Hewie_StateLeapLands_ptmf", "g1_003B1708", "0x100") G1_ALIAS("D_003B180C", "g1_003B1708", "0x104") G1_ALIAS("D_003B1810", "g1_003B1708", "0x108") G1_ALIAS("Hewie_StateBiteHold_ptmf", "g1_003B1708", "0x110") G1_ALIAS("D_003B181C", "g1_003B1708", "0x114") G1_ALIAS("D_003B1820", "g1_003B1708", "0x118") G1_ALIAS("Hewie_StateClamped_ptmf", "g1_003B1708", "0x120") G1_ALIAS("D_003B182C", "g1_003B1708", "0x124") G1_ALIAS("D_003B1830", "g1_003B1708", "0x128") G1_ALIAS("Hewie_StateClamped_ptmf2", "g1_003B1708", "0x130") G1_ALIAS("D_003B183C", "g1_003B1708", "0x134") G1_ALIAS("D_003B1840", "g1_003B1708", "0x138") G1_ALIAS("Hewie_StateHanging_ptmf", "g1_003B1708", "0x140") G1_ALIAS("D_003B184C", "g1_003B1708", "0x144") G1_ALIAS("D_003B1850", "g1_003B1708", "0x148") G1_ALIAS("Hewie_StateClamped_ptmf3", "g1_003B1708", "0x150") G1_ALIAS("D_003B185C", "g1_003B1708", "0x154") G1_ALIAS("D_003B1860", "g1_003B1708", "0x158") G1_ALIAS("Hewie_StateHanging_ptmf2", "g1_003B1708", "0x160") G1_ALIAS("D_003B186C", "g1_003B1708", "0x164") G1_ALIAS("D_003B1870", "g1_003B1708", "0x168") G1_ALIAS("Hewie_StateClamped_ptmf4", "g1_003B1708", "0x170") G1_ALIAS("D_003B187C", "g1_003B1708", "0x174") G1_ALIAS("D_003B1880", "g1_003B1708", "0x178") G1_ALIAS("Hewie_StateJumpArc_ptmf", "g1_003B1708", "0x180") G1_ALIAS("D_003B188C", "g1_003B1708", "0x184") G1_ALIAS("D_003B1890", "g1_003B1708", "0x188") G1_ALIAS("Hewie_StateJumpArc_ptmf2", "g1_003B1708", "0x190") G1_ALIAS("D_003B189C", "g1_003B1708", "0x194") G1_ALIAS("D_003B18A0", "g1_003B1708", "0x198") G1_ALIAS("Hewie_State18B0_ptmf", "g1_003B1708", "0x1A0") G1_ALIAS("D_003B18AC", "g1_003B1708", "0x1A4") G1_ALIAS("D_003B18B0", "g1_003B1708", "0x1A8") G1_ALIAS("Hewie_StateFalling_ptmf", "g1_003B1708", "0x1B0") G1_ALIAS("D_003B18BC", "g1_003B1708", "0x1B4") G1_ALIAS("D_003B18C0", "g1_003B1708", "0x1B8") G1_ALIAS("Hewie_State18B0_ptmf2", "g1_003B1708", "0x1C0") G1_ALIAS("D_003B18CC", "g1_003B1708", "0x1C4") G1_ALIAS("D_003B18D0", "g1_003B1708", "0x1C8") G1_ALIAS("Hewie_StateTackle_ptmf", "g1_003B1708", "0x1D0") G1_ALIAS("D_003B18DC", "g1_003B1708", "0x1D4") G1_ALIAS("D_003B18E0", "g1_003B1708", "0x1D8") G1_ALIAS("Hewie_StateLeapBite_ptmf", "g1_003B1708", "0x1E0") G1_ALIAS("D_003B18EC", "g1_003B1708", "0x1E4") G1_ALIAS("D_003B18F0", "g1_003B1708", "0x1E8") G1_ALIAS("Hewie_StateLanding_ptmf", "g1_003B1708", "0x1F0") G1_ALIAS("D_003B18FC", "g1_003B1708", "0x1F4") G1_ALIAS("D_003B1900", "g1_003B1708", "0x1F8") G1_ALIAS("Hewie_StateLanding_ptmf2", "g1_003B1708", "0x200") G1_ALIAS("D_003B190C", "g1_003B1708", "0x204") G1_ALIAS("D_003B1910", "g1_003B1708", "0x208") G1_ALIAS("Hewie_State1920_ptmf", "g1_003B1708", "0x210") G1_ALIAS("D_003B191C", "g1_003B1708", "0x214") G1_ALIAS("D_003B1920", "g1_003B1708", "0x218") G1_ALIAS("Hewie_StateOnEvent20_ptmf", "g1_003B1708", "0x220") G1_ALIAS("D_003B192C", "g1_003B1708", "0x224") G1_ALIAS("D_003B1930", "g1_003B1708", "0x228") G1_ALIAS("Hewie_StateWalkHeading_ptmf", "g1_003B1708", "0x230") G1_ALIAS("D_003B193C", "g1_003B1708", "0x234") G1_ALIAS("D_003B1940", "g1_003B1708", "0x238") G1_ALIAS("Hewie_StateAnimNext_ptmf", "g1_003B1708", "0x240") G1_ALIAS("D_003B194C", "g1_003B1708", "0x244") G1_ALIAS("D_003B1950", "g1_003B1708", "0x248") G1_ALIAS("Hewie_StateAnimNext_ptmf2", "g1_003B1708", "0x250") G1_ALIAS("D_003B195C", "g1_003B1708", "0x254") G1_ALIAS("D_003B1960", "g1_003B1708", "0x258") G1_ALIAS("Hewie_StateRush_ptmf", "g1_003B1708", "0x260") G1_ALIAS("D_003B196C", "g1_003B1708", "0x264") G1_ALIAS("D_003B1970", "g1_003B1708", "0x268") G1_ALIAS("Hewie_State1980_ptmf", "g1_003B1708", "0x270") G1_ALIAS("D_003B197C", "g1_003B1708", "0x274") G1_ALIAS("D_003B1980", "g1_003B1708", "0x278") G1_ALIAS("Hewie_State1990_ptmf", "g1_003B1708", "0x280") G1_ALIAS("D_003B198C", "g1_003B1708", "0x284") G1_ALIAS("D_003B1990", "g1_003B1708", "0x288") G1_ALIAS("Hewie_State19A0_ptmf", "g1_003B1708", "0x290") G1_ALIAS("D_003B199C", "g1_003B1708", "0x294") G1_ALIAS("D_003B19A0", "g1_003B1708", "0x298") G1_ALIAS("Hewie_State19B0_ptmf", "g1_003B1708", "0x2A0") G1_ALIAS("D_003B19AC", "g1_003B1708", "0x2A4") G1_ALIAS("D_003B19B0", "g1_003B1708", "0x2A8") G1_ALIAS("Hewie_StateBackToNormal_ptmf", "g1_003B1708", "0x2B0") G1_ALIAS("D_003B19BC", "g1_003B1708", "0x2B4") G1_ALIAS("D_003B19C0", "g1_003B1708", "0x2B8") G1_ALIAS("Hewie_StateBehindFiona_ptmf", "g1_003B1708", "0x2C0") G1_ALIAS("D_003B19CC", "g1_003B1708", "0x2C4") G1_ALIAS("D_003B19D0", "g1_003B1708", "0x2C8") G1_ALIAS("Hewie_StateByFiona_ptmf", "g1_003B1708", "0x2D0") G1_ALIAS("D_003B19DC", "g1_003B1708", "0x2D4") G1_ALIAS("D_003B19E0", "g1_003B1708", "0x2D8") G1_ALIAS("Hewie_StatePlayToEnd_ptmf", "g1_003B1708", "0x2E0") G1_ALIAS("D_003B19EC", "g1_003B1708", "0x2E4") G1_ALIAS("D_003B19F0", "g1_003B1708", "0x2E8") G1_ALIAS("Hewie_StateGoForPursuer_ptmf", "g1_003B1708", "0x2F0") G1_ALIAS("D_003B19FC", "g1_003B1708", "0x2F4") G1_ALIAS("D_003B1A00", "g1_003B1708", "0x2F8") G1_ALIAS("Hewie_StateLeapLaunch_ptmf", "g1_003B1708", "0x300") G1_ALIAS("D_003B1A0C", "g1_003B1708", "0x304") G1_ALIAS("D_003B1A10", "g1_003B1708", "0x308") G1_ALIAS("Hewie_StateLeapTakeOff_ptmf", "g1_003B1708", "0x310") G1_ALIAS("D_003B1A1C", "g1_003B1708", "0x314") G1_ALIAS("D_003B1A20", "g1_003B1708", "0x318") G1_ALIAS("Hewie_StateLeapAtPursuer_ptmf", "g1_003B1708", "0x320") G1_ALIAS("D_003B1A2C", "g1_003B1708", "0x324") G1_ALIAS("D_003B1A30", "g1_003B1708", "0x328") G1_ALIAS("Hewie_StateBiteHold_ptmf2", "g1_003B1708", "0x330") G1_ALIAS("D_003B1A3C", "g1_003B1708", "0x334") G1_ALIAS("D_003B1A40", "g1_003B1708", "0x338") G1_ALIAS("Hewie_StateSlideToCommand_ptmf", "g1_003B1708", "0x340") G1_ALIAS("D_003B1A4C", "g1_003B1708", "0x344") G1_ALIAS("D_003B1A50", "g1_003B1708", "0x348") G1_ALIAS("Hewie_State1A60_ptmf", "g1_003B1708", "0x350") G1_ALIAS("D_003B1A5C", "g1_003B1708", "0x354") G1_ALIAS("D_003B1A60", "g1_003B1708", "0x358") G1_ALIAS("Hewie_StateSlideToCommand_ptmf2", "g1_003B1708", "0x360") G1_ALIAS("D_003B1A6C", "g1_003B1708", "0x364") G1_ALIAS("D_003B1A70", "g1_003B1708", "0x368") G1_ALIAS("Hewie_State1A60_ptmf2", "g1_003B1708", "0x370") G1_ALIAS("D_003B1A7C", "g1_003B1708", "0x374") G1_ALIAS("D_003B1A80", "g1_003B1708", "0x378") G1_ALIAS("Hewie_StateSlideToCommand_ptmf3", "g1_003B1708", "0x380") G1_ALIAS("D_003B1A8C", "g1_003B1708", "0x384") G1_ALIAS("D_003B1A90", "g1_003B1708", "0x388") G1_ALIAS("Hewie_State1AA0_ptmf", "g1_003B1708", "0x390") G1_ALIAS("D_003B1A9C", "g1_003B1708", "0x394") G1_ALIAS("D_003B1AA0", "g1_003B1708", "0x398") G1_ALIAS("Hewie_State1A60_ptmf3", "g1_003B1708", "0x3A0") G1_ALIAS("D_003B1AAC", "g1_003B1708", "0x3A4") G1_ALIAS("D_003B1AB0", "g1_003B1708", "0x3A8") G1_ALIAS("Hewie_State1A60_ptmf4", "g1_003B1708", "0x3B0") G1_ALIAS("D_003B1ABC", "g1_003B1708", "0x3B4") G1_ALIAS("D_003B1AC0", "g1_003B1708", "0x3B8") G1_ALIAS("Hewie_State1A60_ptmf5", "g1_003B1708", "0x3C0") G1_ALIAS("D_003B1ACC", "g1_003B1708", "0x3C4") G1_ALIAS("D_003B1AD0", "g1_003B1708", "0x3C8") G1_ALIAS("Hewie_State1A60_ptmf6", "g1_003B1708", "0x3D0") G1_ALIAS("D_003B1ADC", "g1_003B1708", "0x3D4") G1_ALIAS("D_003B1AE0", "g1_003B1708", "0x3D8") G1_ALIAS("Hewie_State1AF0_ptmf", "g1_003B1708", "0x3E0") G1_ALIAS("D_003B1AEC", "g1_003B1708", "0x3E4") G1_ALIAS("D_003B1AF0", "g1_003B1708", "0x3E8") G1_ALIAS("Hewie_State1B00_ptmf", "g1_003B1708", "0x3F0") G1_ALIAS("D_003B1AFC", "g1_003B1708", "0x3F4") G1_ALIAS("D_003B1B00", "g1_003B1708", "0x3F8") G1_ALIAS("Hewie_StateFollowCommand_ptmf", "g1_003B1708", "0x400") G1_ALIAS("D_003B1B0C", "g1_003B1708", "0x404") G1_ALIAS("D_003B1B10", "g1_003B1708", "0x408") G1_ALIAS("Hewie_State1AF0_ptmf2", "g1_003B1708", "0x410") G1_ALIAS("D_003B1B1C", "g1_003B1708", "0x414") G1_ALIAS("D_003B1B20", "g1_003B1708", "0x418") G1_ALIAS("Hewie_State1B00_ptmf2", "g1_003B1708", "0x420") G1_ALIAS("D_003B1B2C", "g1_003B1708", "0x424") G1_ALIAS("D_003B1B30", "g1_003B1708", "0x428") G1_ALIAS("Hewie_State1B40_ptmf", "g1_003B1708", "0x430") G1_ALIAS("D_003B1B3C", "g1_003B1708", "0x434") G1_ALIAS("D_003B1B40", "g1_003B1708", "0x438") G1_ALIAS("Hewie_State1A60_ptmf7", "g1_003B1708", "0x440") G1_ALIAS("D_003B1B4C", "g1_003B1708", "0x444") G1_ALIAS("D_003B1B50", "g1_003B1708", "0x448") G1_ALIAS("Hewie_State1B60_ptmf", "g1_003B1708", "0x450") G1_ALIAS("D_003B1B5C", "g1_003B1708", "0x454") G1_ALIAS("D_003B1B60", "g1_003B1708", "0x458") G1_ALIAS("Hewie_State1B00_ptmf3", "g1_003B1708", "0x460") G1_ALIAS("D_003B1B6C", "g1_003B1708", "0x464") G1_ALIAS("D_003B1B70", "g1_003B1708", "0x468") G1_ALIAS("Hewie_State1A60_ptmf8", "g1_003B1708", "0x470") G1_ALIAS("D_003B1B7C", "g1_003B1708", "0x474") G1_ALIAS("D_003B1B80", "g1_003B1708", "0x478") G1_ALIAS("Hewie_StateGuard_ptmf", "g1_003B1708", "0x480") G1_ALIAS("D_003B1B8C", "g1_003B1708", "0x484") G1_ALIAS("D_003B1B90", "g1_003B1708", "0x488") G1_ALIAS("Hewie_StateFlee_ptmf", "g1_003B1708", "0x490") G1_ALIAS("D_003B1B9C", "g1_003B1708", "0x494") G1_ALIAS("D_003B1BA0", "g1_003B1708", "0x498") G1_ALIAS("Hewie_StateRunSpot_ptmf", "g1_003B1708", "0x4A0") G1_ALIAS("D_003B1BAC", "g1_003B1708", "0x4A4") G1_ALIAS("D_003B1BB0", "g1_003B1708", "0x4A8") G1_ALIAS("Hewie_StatePursuerNear_ptmf", "g1_003B1708", "0x4B0") G1_ALIAS("D_003B1BBC", "g1_003B1708", "0x4B4") G1_ALIAS("D_003B1BC0", "g1_003B1708", "0x4B8") G1_ALIAS("Hewie_StatePlanPursuer_ptmf", "g1_003B1708", "0x4C0") G1_ALIAS("D_003B1BCC", "g1_003B1708", "0x4C4") G1_ALIAS("D_003B1BD0", "g1_003B1708", "0x4C8") G1_ALIAS("Hewie_StateGuard_ptmf2", "g1_003B1708", "0x4D0") G1_ALIAS("D_003B1BDC", "g1_003B1708", "0x4D4") G1_ALIAS("D_003B1BE0", "g1_003B1708", "0x4D8") G1_ALIAS("Hewie_StatePlanPursuer_ptmf2", "g1_003B1708", "0x4E0") G1_ALIAS("D_003B1BEC", "g1_003B1708", "0x4E4") G1_ALIAS("D_003B1BF0", "g1_003B1708", "0x4E8"));

/* 0x003B1C20: Hewie_StateLeap_ptmf, D_003B1C24, D_003B1C28, Hewie_StateTurnTo_ptmf, D_003B1C34, D_003B1C38 ... */
void *g1_003B1C20[80] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateLeap, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateTurnTo, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1C48, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1C58, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateWalkPath2, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateWalkPath2, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateWalkPath, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1C98, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateTurning, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1C98, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1CC8, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1CC8, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateWander, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateHeadForSpot, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StatePraised, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1D18, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateTackle, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1D38, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateLookLow, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateLookLow, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Hewie_StateLeap_ptmf", "g1_003B1C20", "0x0") G1_ALIAS("D_003B1C24", "g1_003B1C20", "0x4") G1_ALIAS("D_003B1C28", "g1_003B1C20", "0x8") G1_ALIAS("Hewie_StateTurnTo_ptmf", "g1_003B1C20", "0x10") G1_ALIAS("D_003B1C34", "g1_003B1C20", "0x14") G1_ALIAS("D_003B1C38", "g1_003B1C20", "0x18") G1_ALIAS("Hewie_State1C48_ptmf", "g1_003B1C20", "0x20") G1_ALIAS("D_003B1C44", "g1_003B1C20", "0x24") G1_ALIAS("D_003B1C48", "g1_003B1C20", "0x28") G1_ALIAS("Hewie_State1C58_ptmf", "g1_003B1C20", "0x30") G1_ALIAS("D_003B1C54", "g1_003B1C20", "0x34") G1_ALIAS("D_003B1C58", "g1_003B1C20", "0x38") G1_ALIAS("Hewie_StateWalkPath2_ptmf", "g1_003B1C20", "0x40") G1_ALIAS("D_003B1C64", "g1_003B1C20", "0x44") G1_ALIAS("D_003B1C68", "g1_003B1C20", "0x48") G1_ALIAS("Hewie_StateWalkPath2_ptmf2", "g1_003B1C20", "0x50") G1_ALIAS("D_003B1C74", "g1_003B1C20", "0x54") G1_ALIAS("D_003B1C78", "g1_003B1C20", "0x58") G1_ALIAS("Hewie_StateWalkPath_ptmf", "g1_003B1C20", "0x60") G1_ALIAS("D_003B1C84", "g1_003B1C20", "0x64") G1_ALIAS("D_003B1C88", "g1_003B1C20", "0x68") G1_ALIAS("Hewie_State1C98_ptmf", "g1_003B1C20", "0x70") G1_ALIAS("D_003B1C94", "g1_003B1C20", "0x74") G1_ALIAS("D_003B1C98", "g1_003B1C20", "0x78") G1_ALIAS("Hewie_StateTurning_ptmf", "g1_003B1C20", "0x80") G1_ALIAS("D_003B1CA4", "g1_003B1C20", "0x84") G1_ALIAS("D_003B1CA8", "g1_003B1C20", "0x88") G1_ALIAS("Hewie_State1C98_ptmf2", "g1_003B1C20", "0x90") G1_ALIAS("D_003B1CB4", "g1_003B1C20", "0x94") G1_ALIAS("D_003B1CB8", "g1_003B1C20", "0x98") G1_ALIAS("Hewie_State1CC8_ptmf", "g1_003B1C20", "0xA0") G1_ALIAS("D_003B1CC4", "g1_003B1C20", "0xA4") G1_ALIAS("D_003B1CC8", "g1_003B1C20", "0xA8") G1_ALIAS("Hewie_State1CC8_ptmf2", "g1_003B1C20", "0xB0") G1_ALIAS("D_003B1CD4", "g1_003B1C20", "0xB4") G1_ALIAS("D_003B1CD8", "g1_003B1C20", "0xB8") G1_ALIAS("Hewie_StateWander_ptmf", "g1_003B1C20", "0xC0") G1_ALIAS("D_003B1CE4", "g1_003B1C20", "0xC4") G1_ALIAS("D_003B1CE8", "g1_003B1C20", "0xC8") G1_ALIAS("Hewie_StateHeadForSpot_ptmf", "g1_003B1C20", "0xD0") G1_ALIAS("D_003B1CF4", "g1_003B1C20", "0xD4") G1_ALIAS("D_003B1CF8", "g1_003B1C20", "0xD8") G1_ALIAS("Hewie_StatePraised_ptmf", "g1_003B1C20", "0xE0") G1_ALIAS("D_003B1D04", "g1_003B1C20", "0xE4") G1_ALIAS("D_003B1D08", "g1_003B1C20", "0xE8") G1_ALIAS("Hewie_State1D18_ptmf", "g1_003B1C20", "0xF0") G1_ALIAS("D_003B1D14", "g1_003B1C20", "0xF4") G1_ALIAS("D_003B1D18", "g1_003B1C20", "0xF8") G1_ALIAS("Hewie_StateTackle_ptmf2", "g1_003B1C20", "0x100") G1_ALIAS("D_003B1D24", "g1_003B1C20", "0x104") G1_ALIAS("D_003B1D28", "g1_003B1C20", "0x108") G1_ALIAS("Hewie_State1D38_ptmf", "g1_003B1C20", "0x110") G1_ALIAS("D_003B1D34", "g1_003B1C20", "0x114") G1_ALIAS("D_003B1D38", "g1_003B1C20", "0x118") G1_ALIAS("Hewie_StateLookLow_ptmf", "g1_003B1C20", "0x120") G1_ALIAS("D_003B1D44", "g1_003B1C20", "0x124") G1_ALIAS("D_003B1D48", "g1_003B1C20", "0x128") G1_ALIAS("Hewie_StateLookLow_ptmf2", "g1_003B1C20", "0x130") G1_ALIAS("D_003B1D54", "g1_003B1C20", "0x134") G1_ALIAS("D_003B1D58", "g1_003B1C20", "0x138"));

/* 0x003B1D80: D_003B1D80, D_003B1D84, D_003B1D88, D_003B1D90, D_003B1D94, D_003B1D98 */
void *g1_003B1D80[8] __attribute__((aligned(128), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1D88, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1D98, (void *)0x00000000u,
};
__asm__(G1_ALIAS("D_003B1D80", "g1_003B1D80", "0x0") G1_ALIAS("D_003B1D84", "g1_003B1D80", "0x4") G1_ALIAS("D_003B1D88", "g1_003B1D80", "0x8") G1_ALIAS("D_003B1D90", "g1_003B1D80", "0x10") G1_ALIAS("D_003B1D94", "g1_003B1D80", "0x14") G1_ALIAS("D_003B1D98", "g1_003B1D80", "0x18"));

/* 0x003B1DC0: D_003B1DC0, D_003B1DC4, D_003B1DC8 */
void *g1_003B1DC0[4] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1DC8, (void *)0x00000000u,
};
__asm__(G1_ALIAS("D_003B1DC0", "g1_003B1DC0", "0x0") G1_ALIAS("D_003B1DC4", "g1_003B1DC0", "0x4") G1_ALIAS("D_003B1DC8", "g1_003B1DC0", "0x8"));

/* 0x003B1DF0: D_003B1DF0, D_003B1DF4, D_003B1DF8, D_003B1E00, D_003B1E04, D_003B1E08 ... */
void *g1_003B1DF0[348] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateSteer, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1DC8, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1DC8, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StatePose4, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StatePose4, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1E48, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateFaceScent, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateFaceTarget, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateBarkAtTarget, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateWaitForFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateBarkAtFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateSlideToFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateSlideToFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateSlideToFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateKeepAway, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateKeepAway, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateFlank, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateScramble, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateSetOff, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1F28, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateRoam, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateComeToCommand, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateTricks, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateTricks, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateTricks, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1F88, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateRunAtPursuer, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateCloseOnTarget, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateGoToExit, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateGoToExit, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateAfter27, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateAfter29, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateAfter2B, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateToDoor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State2018, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State2018, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State2018, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State2018, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateToDoor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateToDoor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateLoudNoise, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateToDoor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State2098, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State20A8, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateToDefault, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State20C8, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateRootMotion, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateRootMotion, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateRootMotion, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateRootMotion, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateSetOff, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateSetOff, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State2138, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateTurnStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateTurnStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State2168, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateHeadForSpot, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateTurnWithFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State2198, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateKnockedDown, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateKeepNear, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateKeepNear, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateKeepNear, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StatePose2, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateSqueeze, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateWalkOut, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateStepAwayFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateMoveAlong, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateMoveAlong, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateOffMesh, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateMoveAlong, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateMoveAlong, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateCloseIn, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateSlope, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateRunToFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateRun, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State22B8, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateKeepBehind, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateAnimOver, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State22E8, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateTargetPursuer, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StatePlayAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State2318, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateFetch, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateFollowMover, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State1A60, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State2358, (void *)0x00000000u,
};
__asm__(G1_ALIAS("D_003B1DF0", "g1_003B1DF0", "0x0") G1_ALIAS("D_003B1DF4", "g1_003B1DF0", "0x4") G1_ALIAS("D_003B1DF8", "g1_003B1DF0", "0x8") G1_ALIAS("D_003B1E00", "g1_003B1DF0", "0x10") G1_ALIAS("D_003B1E04", "g1_003B1DF0", "0x14") G1_ALIAS("D_003B1E08", "g1_003B1DF0", "0x18") G1_ALIAS("D_003B1E10", "g1_003B1DF0", "0x20") G1_ALIAS("D_003B1E14", "g1_003B1DF0", "0x24") G1_ALIAS("D_003B1E18", "g1_003B1DF0", "0x28") G1_ALIAS("D_003B1E20", "g1_003B1DF0", "0x30") G1_ALIAS("D_003B1E24", "g1_003B1DF0", "0x34") G1_ALIAS("D_003B1E28", "g1_003B1DF0", "0x38") G1_ALIAS("D_003B1E30", "g1_003B1DF0", "0x40") G1_ALIAS("D_003B1E34", "g1_003B1DF0", "0x44") G1_ALIAS("D_003B1E38", "g1_003B1DF0", "0x48") G1_ALIAS("D_003B1E40", "g1_003B1DF0", "0x50") G1_ALIAS("D_003B1E44", "g1_003B1DF0", "0x54") G1_ALIAS("D_003B1E48", "g1_003B1DF0", "0x58") G1_ALIAS("D_003B1E50", "g1_003B1DF0", "0x60") G1_ALIAS("D_003B1E54", "g1_003B1DF0", "0x64") G1_ALIAS("D_003B1E58", "g1_003B1DF0", "0x68") G1_ALIAS("D_003B1E60", "g1_003B1DF0", "0x70") G1_ALIAS("D_003B1E64", "g1_003B1DF0", "0x74") G1_ALIAS("D_003B1E68", "g1_003B1DF0", "0x78") G1_ALIAS("D_003B1E70", "g1_003B1DF0", "0x80") G1_ALIAS("D_003B1E74", "g1_003B1DF0", "0x84") G1_ALIAS("D_003B1E78", "g1_003B1DF0", "0x88") G1_ALIAS("D_003B1E80", "g1_003B1DF0", "0x90") G1_ALIAS("D_003B1E84", "g1_003B1DF0", "0x94") G1_ALIAS("D_003B1E88", "g1_003B1DF0", "0x98") G1_ALIAS("D_003B1E90", "g1_003B1DF0", "0xA0") G1_ALIAS("D_003B1E94", "g1_003B1DF0", "0xA4") G1_ALIAS("D_003B1E98", "g1_003B1DF0", "0xA8") G1_ALIAS("D_003B1EA0", "g1_003B1DF0", "0xB0") G1_ALIAS("D_003B1EA4", "g1_003B1DF0", "0xB4") G1_ALIAS("D_003B1EA8", "g1_003B1DF0", "0xB8") G1_ALIAS("D_003B1EB0", "g1_003B1DF0", "0xC0") G1_ALIAS("D_003B1EB4", "g1_003B1DF0", "0xC4") G1_ALIAS("D_003B1EB8", "g1_003B1DF0", "0xC8") G1_ALIAS("D_003B1EC0", "g1_003B1DF0", "0xD0") G1_ALIAS("D_003B1EC4", "g1_003B1DF0", "0xD4") G1_ALIAS("D_003B1EC8", "g1_003B1DF0", "0xD8") G1_ALIAS("D_003B1ED0", "g1_003B1DF0", "0xE0") G1_ALIAS("D_003B1ED4", "g1_003B1DF0", "0xE4") G1_ALIAS("D_003B1ED8", "g1_003B1DF0", "0xE8") G1_ALIAS("D_003B1EE0", "g1_003B1DF0", "0xF0") G1_ALIAS("D_003B1EE4", "g1_003B1DF0", "0xF4") G1_ALIAS("D_003B1EE8", "g1_003B1DF0", "0xF8") G1_ALIAS("D_003B1EF0", "g1_003B1DF0", "0x100") G1_ALIAS("D_003B1EF4", "g1_003B1DF0", "0x104") G1_ALIAS("D_003B1EF8", "g1_003B1DF0", "0x108") G1_ALIAS("D_003B1F00", "g1_003B1DF0", "0x110") G1_ALIAS("D_003B1F04", "g1_003B1DF0", "0x114") G1_ALIAS("D_003B1F08", "g1_003B1DF0", "0x118") G1_ALIAS("D_003B1F10", "g1_003B1DF0", "0x120") G1_ALIAS("D_003B1F14", "g1_003B1DF0", "0x124") G1_ALIAS("D_003B1F18", "g1_003B1DF0", "0x128") G1_ALIAS("D_003B1F20", "g1_003B1DF0", "0x130") G1_ALIAS("D_003B1F24", "g1_003B1DF0", "0x134") G1_ALIAS("D_003B1F28", "g1_003B1DF0", "0x138") G1_ALIAS("D_003B1F30", "g1_003B1DF0", "0x140") G1_ALIAS("D_003B1F34", "g1_003B1DF0", "0x144") G1_ALIAS("D_003B1F38", "g1_003B1DF0", "0x148") G1_ALIAS("D_003B1F40", "g1_003B1DF0", "0x150") G1_ALIAS("D_003B1F44", "g1_003B1DF0", "0x154") G1_ALIAS("D_003B1F48", "g1_003B1DF0", "0x158") G1_ALIAS("D_003B1F50", "g1_003B1DF0", "0x160") G1_ALIAS("D_003B1F54", "g1_003B1DF0", "0x164") G1_ALIAS("D_003B1F58", "g1_003B1DF0", "0x168") G1_ALIAS("D_003B1F60", "g1_003B1DF0", "0x170") G1_ALIAS("D_003B1F64", "g1_003B1DF0", "0x174") G1_ALIAS("D_003B1F68", "g1_003B1DF0", "0x178") G1_ALIAS("D_003B1F70", "g1_003B1DF0", "0x180") G1_ALIAS("D_003B1F74", "g1_003B1DF0", "0x184") G1_ALIAS("D_003B1F78", "g1_003B1DF0", "0x188") G1_ALIAS("D_003B1F80", "g1_003B1DF0", "0x190") G1_ALIAS("D_003B1F84", "g1_003B1DF0", "0x194") G1_ALIAS("D_003B1F88", "g1_003B1DF0", "0x198") G1_ALIAS("D_003B1F90", "g1_003B1DF0", "0x1A0") G1_ALIAS("D_003B1F94", "g1_003B1DF0", "0x1A4") G1_ALIAS("D_003B1F98", "g1_003B1DF0", "0x1A8") G1_ALIAS("D_003B1FA0", "g1_003B1DF0", "0x1B0") G1_ALIAS("D_003B1FA4", "g1_003B1DF0", "0x1B4") G1_ALIAS("D_003B1FA8", "g1_003B1DF0", "0x1B8") G1_ALIAS("D_003B1FB0", "g1_003B1DF0", "0x1C0") G1_ALIAS("D_003B1FB4", "g1_003B1DF0", "0x1C4") G1_ALIAS("D_003B1FB8", "g1_003B1DF0", "0x1C8") G1_ALIAS("D_003B1FC0", "g1_003B1DF0", "0x1D0") G1_ALIAS("D_003B1FC4", "g1_003B1DF0", "0x1D4") G1_ALIAS("D_003B1FC8", "g1_003B1DF0", "0x1D8") G1_ALIAS("D_003B1FD0", "g1_003B1DF0", "0x1E0") G1_ALIAS("D_003B1FD4", "g1_003B1DF0", "0x1E4") G1_ALIAS("D_003B1FD8", "g1_003B1DF0", "0x1E8") G1_ALIAS("D_003B1FE0", "g1_003B1DF0", "0x1F0") G1_ALIAS("D_003B1FE4", "g1_003B1DF0", "0x1F4") G1_ALIAS("D_003B1FE8", "g1_003B1DF0", "0x1F8") G1_ALIAS("D_003B1FF0", "g1_003B1DF0", "0x200") G1_ALIAS("D_003B1FF4", "g1_003B1DF0", "0x204") G1_ALIAS("D_003B1FF8", "g1_003B1DF0", "0x208") G1_ALIAS("D_003B2000", "g1_003B1DF0", "0x210") G1_ALIAS("D_003B2004", "g1_003B1DF0", "0x214") G1_ALIAS("D_003B2008", "g1_003B1DF0", "0x218") G1_ALIAS("D_003B2010", "g1_003B1DF0", "0x220") G1_ALIAS("D_003B2014", "g1_003B1DF0", "0x224") G1_ALIAS("D_003B2018", "g1_003B1DF0", "0x228") G1_ALIAS("D_003B2020", "g1_003B1DF0", "0x230") G1_ALIAS("D_003B2024", "g1_003B1DF0", "0x234") G1_ALIAS("D_003B2028", "g1_003B1DF0", "0x238") G1_ALIAS("D_003B2030", "g1_003B1DF0", "0x240") G1_ALIAS("D_003B2034", "g1_003B1DF0", "0x244") G1_ALIAS("D_003B2038", "g1_003B1DF0", "0x248") G1_ALIAS("D_003B2040", "g1_003B1DF0", "0x250") G1_ALIAS("D_003B2044", "g1_003B1DF0", "0x254") G1_ALIAS("D_003B2048", "g1_003B1DF0", "0x258") G1_ALIAS("D_003B2050", "g1_003B1DF0", "0x260") G1_ALIAS("D_003B2054", "g1_003B1DF0", "0x264") G1_ALIAS("D_003B2058", "g1_003B1DF0", "0x268") G1_ALIAS("D_003B2060", "g1_003B1DF0", "0x270") G1_ALIAS("D_003B2064", "g1_003B1DF0", "0x274") G1_ALIAS("D_003B2068", "g1_003B1DF0", "0x278") G1_ALIAS("D_003B2070", "g1_003B1DF0", "0x280") G1_ALIAS("D_003B2074", "g1_003B1DF0", "0x284") G1_ALIAS("D_003B2078", "g1_003B1DF0", "0x288") G1_ALIAS("D_003B2080", "g1_003B1DF0", "0x290") G1_ALIAS("D_003B2084", "g1_003B1DF0", "0x294") G1_ALIAS("D_003B2088", "g1_003B1DF0", "0x298") G1_ALIAS("D_003B2090", "g1_003B1DF0", "0x2A0") G1_ALIAS("D_003B2094", "g1_003B1DF0", "0x2A4") G1_ALIAS("D_003B2098", "g1_003B1DF0", "0x2A8") G1_ALIAS("D_003B20A0", "g1_003B1DF0", "0x2B0") G1_ALIAS("D_003B20A4", "g1_003B1DF0", "0x2B4") G1_ALIAS("D_003B20A8", "g1_003B1DF0", "0x2B8") G1_ALIAS("D_003B20B0", "g1_003B1DF0", "0x2C0") G1_ALIAS("D_003B20B4", "g1_003B1DF0", "0x2C4") G1_ALIAS("D_003B20B8", "g1_003B1DF0", "0x2C8") G1_ALIAS("D_003B20C0", "g1_003B1DF0", "0x2D0") G1_ALIAS("D_003B20C4", "g1_003B1DF0", "0x2D4") G1_ALIAS("D_003B20C8", "g1_003B1DF0", "0x2D8") G1_ALIAS("D_003B20D0", "g1_003B1DF0", "0x2E0") G1_ALIAS("D_003B20D4", "g1_003B1DF0", "0x2E4") G1_ALIAS("D_003B20D8", "g1_003B1DF0", "0x2E8") G1_ALIAS("D_003B20E0", "g1_003B1DF0", "0x2F0") G1_ALIAS("D_003B20E4", "g1_003B1DF0", "0x2F4") G1_ALIAS("D_003B20E8", "g1_003B1DF0", "0x2F8") G1_ALIAS("D_003B20F0", "g1_003B1DF0", "0x300") G1_ALIAS("D_003B20F4", "g1_003B1DF0", "0x304") G1_ALIAS("D_003B20F8", "g1_003B1DF0", "0x308") G1_ALIAS("D_003B2100", "g1_003B1DF0", "0x310") G1_ALIAS("D_003B2104", "g1_003B1DF0", "0x314") G1_ALIAS("D_003B2108", "g1_003B1DF0", "0x318") G1_ALIAS("D_003B2110", "g1_003B1DF0", "0x320") G1_ALIAS("D_003B2114", "g1_003B1DF0", "0x324") G1_ALIAS("D_003B2118", "g1_003B1DF0", "0x328") G1_ALIAS("D_003B2120", "g1_003B1DF0", "0x330") G1_ALIAS("D_003B2124", "g1_003B1DF0", "0x334") G1_ALIAS("D_003B2128", "g1_003B1DF0", "0x338") G1_ALIAS("D_003B2130", "g1_003B1DF0", "0x340") G1_ALIAS("D_003B2134", "g1_003B1DF0", "0x344") G1_ALIAS("D_003B2138", "g1_003B1DF0", "0x348") G1_ALIAS("D_003B2140", "g1_003B1DF0", "0x350") G1_ALIAS("D_003B2144", "g1_003B1DF0", "0x354") G1_ALIAS("D_003B2148", "g1_003B1DF0", "0x358") G1_ALIAS("D_003B2150", "g1_003B1DF0", "0x360") G1_ALIAS("D_003B2154", "g1_003B1DF0", "0x364") G1_ALIAS("D_003B2158", "g1_003B1DF0", "0x368") G1_ALIAS("D_003B2160", "g1_003B1DF0", "0x370") G1_ALIAS("D_003B2164", "g1_003B1DF0", "0x374") G1_ALIAS("D_003B2168", "g1_003B1DF0", "0x378") G1_ALIAS("D_003B2170", "g1_003B1DF0", "0x380") G1_ALIAS("D_003B2174", "g1_003B1DF0", "0x384") G1_ALIAS("D_003B2178", "g1_003B1DF0", "0x388") G1_ALIAS("D_003B2180", "g1_003B1DF0", "0x390") G1_ALIAS("D_003B2184", "g1_003B1DF0", "0x394") G1_ALIAS("D_003B2188", "g1_003B1DF0", "0x398") G1_ALIAS("D_003B2190", "g1_003B1DF0", "0x3A0") G1_ALIAS("D_003B2194", "g1_003B1DF0", "0x3A4") G1_ALIAS("D_003B2198", "g1_003B1DF0", "0x3A8") G1_ALIAS("D_003B21A0", "g1_003B1DF0", "0x3B0") G1_ALIAS("D_003B21A4", "g1_003B1DF0", "0x3B4") G1_ALIAS("D_003B21A8", "g1_003B1DF0", "0x3B8") G1_ALIAS("D_003B21B0", "g1_003B1DF0", "0x3C0") G1_ALIAS("D_003B21B4", "g1_003B1DF0", "0x3C4") G1_ALIAS("D_003B21B8", "g1_003B1DF0", "0x3C8") G1_ALIAS("D_003B21C0", "g1_003B1DF0", "0x3D0") G1_ALIAS("D_003B21C4", "g1_003B1DF0", "0x3D4") G1_ALIAS("D_003B21C8", "g1_003B1DF0", "0x3D8") G1_ALIAS("D_003B21D0", "g1_003B1DF0", "0x3E0") G1_ALIAS("D_003B21D4", "g1_003B1DF0", "0x3E4") G1_ALIAS("D_003B21D8", "g1_003B1DF0", "0x3E8") G1_ALIAS("D_003B21E0", "g1_003B1DF0", "0x3F0") G1_ALIAS("D_003B21E4", "g1_003B1DF0", "0x3F4") G1_ALIAS("D_003B21E8", "g1_003B1DF0", "0x3F8") G1_ALIAS("D_003B21F0", "g1_003B1DF0", "0x400") G1_ALIAS("D_003B21F4", "g1_003B1DF0", "0x404") G1_ALIAS("D_003B21F8", "g1_003B1DF0", "0x408") G1_ALIAS("D_003B2200", "g1_003B1DF0", "0x410") G1_ALIAS("D_003B2204", "g1_003B1DF0", "0x414") G1_ALIAS("D_003B2208", "g1_003B1DF0", "0x418") G1_ALIAS("D_003B2210", "g1_003B1DF0", "0x420") G1_ALIAS("D_003B2214", "g1_003B1DF0", "0x424") G1_ALIAS("D_003B2218", "g1_003B1DF0", "0x428") G1_ALIAS("D_003B2220", "g1_003B1DF0", "0x430") G1_ALIAS("D_003B2224", "g1_003B1DF0", "0x434") G1_ALIAS("D_003B2228", "g1_003B1DF0", "0x438") G1_ALIAS("D_003B2230", "g1_003B1DF0", "0x440") G1_ALIAS("D_003B2234", "g1_003B1DF0", "0x444") G1_ALIAS("D_003B2238", "g1_003B1DF0", "0x448") G1_ALIAS("D_003B2240", "g1_003B1DF0", "0x450") G1_ALIAS("D_003B2244", "g1_003B1DF0", "0x454") G1_ALIAS("D_003B2248", "g1_003B1DF0", "0x458") G1_ALIAS("D_003B2250", "g1_003B1DF0", "0x460") G1_ALIAS("D_003B2254", "g1_003B1DF0", "0x464") G1_ALIAS("D_003B2258", "g1_003B1DF0", "0x468") G1_ALIAS("D_003B2260", "g1_003B1DF0", "0x470") G1_ALIAS("D_003B2264", "g1_003B1DF0", "0x474") G1_ALIAS("D_003B2268", "g1_003B1DF0", "0x478") G1_ALIAS("D_003B2270", "g1_003B1DF0", "0x480") G1_ALIAS("D_003B2274", "g1_003B1DF0", "0x484") G1_ALIAS("D_003B2278", "g1_003B1DF0", "0x488") G1_ALIAS("D_003B2280", "g1_003B1DF0", "0x490") G1_ALIAS("D_003B2284", "g1_003B1DF0", "0x494") G1_ALIAS("D_003B2288", "g1_003B1DF0", "0x498") G1_ALIAS("D_003B2290", "g1_003B1DF0", "0x4A0") G1_ALIAS("D_003B2294", "g1_003B1DF0", "0x4A4") G1_ALIAS("D_003B2298", "g1_003B1DF0", "0x4A8") G1_ALIAS("D_003B22A0", "g1_003B1DF0", "0x4B0") G1_ALIAS("D_003B22A4", "g1_003B1DF0", "0x4B4") G1_ALIAS("D_003B22A8", "g1_003B1DF0", "0x4B8") G1_ALIAS("D_003B22B0", "g1_003B1DF0", "0x4C0") G1_ALIAS("D_003B22B4", "g1_003B1DF0", "0x4C4") G1_ALIAS("D_003B22B8", "g1_003B1DF0", "0x4C8") G1_ALIAS("D_003B22C0", "g1_003B1DF0", "0x4D0") G1_ALIAS("D_003B22C4", "g1_003B1DF0", "0x4D4") G1_ALIAS("D_003B22C8", "g1_003B1DF0", "0x4D8") G1_ALIAS("D_003B22D0", "g1_003B1DF0", "0x4E0") G1_ALIAS("D_003B22D4", "g1_003B1DF0", "0x4E4") G1_ALIAS("D_003B22D8", "g1_003B1DF0", "0x4E8") G1_ALIAS("D_003B22E0", "g1_003B1DF0", "0x4F0") G1_ALIAS("D_003B22E4", "g1_003B1DF0", "0x4F4") G1_ALIAS("D_003B22E8", "g1_003B1DF0", "0x4F8") G1_ALIAS("D_003B22F0", "g1_003B1DF0", "0x500") G1_ALIAS("D_003B22F4", "g1_003B1DF0", "0x504") G1_ALIAS("D_003B22F8", "g1_003B1DF0", "0x508") G1_ALIAS("D_003B2300", "g1_003B1DF0", "0x510") G1_ALIAS("D_003B2304", "g1_003B1DF0", "0x514") G1_ALIAS("D_003B2308", "g1_003B1DF0", "0x518") G1_ALIAS("D_003B2310", "g1_003B1DF0", "0x520") G1_ALIAS("D_003B2314", "g1_003B1DF0", "0x524") G1_ALIAS("D_003B2318", "g1_003B1DF0", "0x528") G1_ALIAS("D_003B2320", "g1_003B1DF0", "0x530") G1_ALIAS("D_003B2324", "g1_003B1DF0", "0x534") G1_ALIAS("D_003B2328", "g1_003B1DF0", "0x538") G1_ALIAS("D_003B2330", "g1_003B1DF0", "0x540") G1_ALIAS("D_003B2334", "g1_003B1DF0", "0x544") G1_ALIAS("D_003B2338", "g1_003B1DF0", "0x548") G1_ALIAS("D_003B2340", "g1_003B1DF0", "0x550") G1_ALIAS("D_003B2344", "g1_003B1DF0", "0x554") G1_ALIAS("D_003B2348", "g1_003B1DF0", "0x558") G1_ALIAS("D_003B2350", "g1_003B1DF0", "0x560") G1_ALIAS("D_003B2354", "g1_003B1DF0", "0x564") G1_ALIAS("D_003B2358", "g1_003B1DF0", "0x568"));

/* 0x003B2380: D_003B2380, D_003B2384, D_003B2388, D_003B2390, D_003B2394, D_003B2398 ... */
void *g1_003B2380[52] __attribute__((aligned(128), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State2388, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StatePlanPursuer, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateKeepNear, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateWhine, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateRunForSpot, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State23D8, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StatePose4, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateStepAwayTarget, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateSteered, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_State2418, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateBark, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateBackOnMesh, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Hewie_StateRunUpJump, (void *)0x00000000u,
};
__asm__(G1_ALIAS("D_003B2380", "g1_003B2380", "0x0") G1_ALIAS("D_003B2384", "g1_003B2380", "0x4") G1_ALIAS("D_003B2388", "g1_003B2380", "0x8") G1_ALIAS("D_003B2390", "g1_003B2380", "0x10") G1_ALIAS("D_003B2394", "g1_003B2380", "0x14") G1_ALIAS("D_003B2398", "g1_003B2380", "0x18") G1_ALIAS("D_003B23A0", "g1_003B2380", "0x20") G1_ALIAS("D_003B23A4", "g1_003B2380", "0x24") G1_ALIAS("D_003B23A8", "g1_003B2380", "0x28") G1_ALIAS("D_003B23B0", "g1_003B2380", "0x30") G1_ALIAS("D_003B23B4", "g1_003B2380", "0x34") G1_ALIAS("D_003B23B8", "g1_003B2380", "0x38") G1_ALIAS("D_003B23C0", "g1_003B2380", "0x40") G1_ALIAS("D_003B23C4", "g1_003B2380", "0x44") G1_ALIAS("D_003B23C8", "g1_003B2380", "0x48") G1_ALIAS("D_003B23D0", "g1_003B2380", "0x50") G1_ALIAS("D_003B23D4", "g1_003B2380", "0x54") G1_ALIAS("D_003B23D8", "g1_003B2380", "0x58") G1_ALIAS("D_003B23E0", "g1_003B2380", "0x60") G1_ALIAS("D_003B23E4", "g1_003B2380", "0x64") G1_ALIAS("D_003B23E8", "g1_003B2380", "0x68") G1_ALIAS("D_003B23F0", "g1_003B2380", "0x70") G1_ALIAS("D_003B23F4", "g1_003B2380", "0x74") G1_ALIAS("D_003B23F8", "g1_003B2380", "0x78") G1_ALIAS("D_003B2400", "g1_003B2380", "0x80") G1_ALIAS("D_003B2404", "g1_003B2380", "0x84") G1_ALIAS("D_003B2408", "g1_003B2380", "0x88") G1_ALIAS("D_003B2410", "g1_003B2380", "0x90") G1_ALIAS("D_003B2414", "g1_003B2380", "0x94") G1_ALIAS("D_003B2418", "g1_003B2380", "0x98") G1_ALIAS("D_003B2420", "g1_003B2380", "0xA0") G1_ALIAS("D_003B2424", "g1_003B2380", "0xA4") G1_ALIAS("D_003B2428", "g1_003B2380", "0xA8") G1_ALIAS("D_003B2430", "g1_003B2380", "0xB0") G1_ALIAS("D_003B2434", "g1_003B2380", "0xB4") G1_ALIAS("D_003B2438", "g1_003B2380", "0xB8") G1_ALIAS("D_003B2440", "g1_003B2380", "0xC0") G1_ALIAS("D_003B2444", "g1_003B2380", "0xC4") G1_ALIAS("D_003B2448", "g1_003B2380", "0xC8"));

/* 0x003B25A8: Fiona_StateIdleMove_ptmf, D_003B25B0, Fiona_StateIdleStep_ptmf, D_003B25C0, Fiona_StateIdleStep_ptmf2, D_003B25D0 ... */
void *g1_003B25A8[614] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateIdleMove, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateIdleStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateIdleStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCmdDone, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateRequestTurn, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateScriptDoor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateDoorStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLadder, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateKickStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateKickStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateShoveStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCaught, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCaught, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateFleeStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLetGoStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateIdleStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCmdDone, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateIdleStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateDoorStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateDoorStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCmdWalkSpot, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCmdPathSpot, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCmdPathTo, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateTurnOnSpot, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateTurnOnSpot, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCmdWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StatePanicFall, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateIdleAtEvent2, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StatePanicAttack, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateActionOver, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateTurnStick, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateSpecialEntry, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateThrown, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StatePush, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StatePushLetGo, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StatePushStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StatePushLetGo, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StatePushLetGo, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLadderWalk, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateOnLadder, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCaughtCrawling, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLadderOff, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLadderOff, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLadderOff, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLadderOff, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLockedSlow, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLockedSpot, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLockedQuick, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateDoorWalk, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateDoorAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateDoorStepOut, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLockedTry, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateIdleAtEvent2, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateIdleAtEvent2, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateScriptDoorWalk, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateScriptDoorOpened, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateStrike, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateShove, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateKick, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateAfterCaught, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCrawlDrop, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCrawlPull, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCrawlFall, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCrawlDown, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCrawlPull, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLedHand, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateHandSpot, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StatePulledFree, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateDragged, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StatePulledFree, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StatePulledFree, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StatePulledFree, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateDraggedOff, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StatePulledFree, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StatePulledFree, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLedAway, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLedSpot, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateAfterThrow, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateGetUp, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateSpecialEntry, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateGetUp, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateSpecialEntry, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateAfterThrow, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateAfterKnockDown, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateFallen, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateSpecialEntry, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateHeld, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLookEnd, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLookEnd, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateWalkThenGesture, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLookEnd, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLookEnd, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLookEnd, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateHeldStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateHeldStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateActionOver, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateHeldStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateActionOver, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLookEnd, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLookAround, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateActionOver, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateActionOver, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateHeldWalk, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateHeldStopped, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateWaitHewie, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateHeldGesture, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateHeldGesture, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateHeldGesture, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateWaitHewie, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateHeldCommand, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateGestureLeadIn, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateGesture, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateGestureEnd, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateTurnUntilDone, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateIdleAtEvent, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLetGo, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateFleeTurn, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateStruggle, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateAfterFall, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateOnGround, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCmdPathTurn, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCmdAnimDone, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCmdTurn, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCmdAnimDone, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCmdPath, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCmdToSpot, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateTurnStanding, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateThrown, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateKnockedDown, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCaught, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateGrabbed, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateCaughtCrawling, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLedByHand, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateLedWalking, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateThrown, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateHitByDoor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateKnockedDown, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateHold, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateFall13, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Fiona_StateDoorStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)PathPlan_StepBreadth, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)PathPlan_StepDepth, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)PathPlan_StepDijkstra, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)PathPlan_StepGreedySorted, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)PathPlan_StepAStar, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)PathPlan_StepBounded, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)PathPlan_StepGreedy, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)PathPlan_StepCheapNear, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)PathPlan_StepAtGoal, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Fiona_StateIdleMove_ptmf", "g1_003B25A8", "0x0") G1_ALIAS("D_003B25B0", "g1_003B25A8", "0x8") G1_ALIAS("Fiona_StateIdleStep_ptmf", "g1_003B25A8", "0x10") G1_ALIAS("D_003B25C0", "g1_003B25A8", "0x18") G1_ALIAS("Fiona_StateIdleStep_ptmf2", "g1_003B25A8", "0x20") G1_ALIAS("D_003B25D0", "g1_003B25A8", "0x28") G1_ALIAS("Fiona_StateCmdDone_ptmf", "g1_003B25A8", "0x30") G1_ALIAS("D_003B25E0", "g1_003B25A8", "0x38") G1_ALIAS("Fiona_StateRequestTurn_ptmf", "g1_003B25A8", "0x40") G1_ALIAS("D_003B25F0", "g1_003B25A8", "0x48") G1_ALIAS("Fiona_StateScriptDoor_ptmf", "g1_003B25A8", "0x50") G1_ALIAS("D_003B2600", "g1_003B25A8", "0x58") G1_ALIAS("Fiona_StateDoorStart_ptmf", "g1_003B25A8", "0x60") G1_ALIAS("D_003B2610", "g1_003B25A8", "0x68") G1_ALIAS("Fiona_StateLadder_ptmf", "g1_003B25A8", "0x70") G1_ALIAS("D_003B2620", "g1_003B25A8", "0x78") G1_ALIAS("Fiona_StateKickStart_ptmf", "g1_003B25A8", "0x80") G1_ALIAS("D_003B2630", "g1_003B25A8", "0x88") G1_ALIAS("Fiona_StateKickStart_ptmf2", "g1_003B25A8", "0x90") G1_ALIAS("D_003B2640", "g1_003B25A8", "0x98") G1_ALIAS("Fiona_StateShoveStart_ptmf", "g1_003B25A8", "0xA0") G1_ALIAS("D_003B2650", "g1_003B25A8", "0xA8") G1_ALIAS("Fiona_StateCaught_ptmf", "g1_003B25A8", "0xB0") G1_ALIAS("D_003B2660", "g1_003B25A8", "0xB8") G1_ALIAS("Fiona_StateCaught_ptmf2", "g1_003B25A8", "0xC0") G1_ALIAS("D_003B2670", "g1_003B25A8", "0xC8") G1_ALIAS("Fiona_StateFleeStart_ptmf", "g1_003B25A8", "0xD0") G1_ALIAS("D_003B2680", "g1_003B25A8", "0xD8") G1_ALIAS("Fiona_StateLetGoStart_ptmf", "g1_003B25A8", "0xE0") G1_ALIAS("D_003B2690", "g1_003B25A8", "0xE8") G1_ALIAS("Fiona_StateIdleStep_ptmf3", "g1_003B25A8", "0xF0") G1_ALIAS("D_003B26A0", "g1_003B25A8", "0xF8") G1_ALIAS("Fiona_StateCmdDone_ptmf2", "g1_003B25A8", "0x100") G1_ALIAS("D_003B26B0", "g1_003B25A8", "0x108") G1_ALIAS("Fiona_StateIdleStep_ptmf4", "g1_003B25A8", "0x110") G1_ALIAS("D_003B26C0", "g1_003B25A8", "0x118") G1_ALIAS("Fiona_StateDoorStart_ptmf2", "g1_003B25A8", "0x120") G1_ALIAS("D_003B26D0", "g1_003B25A8", "0x128") G1_ALIAS("Fiona_StateDoorStart_ptmf3", "g1_003B25A8", "0x130") G1_ALIAS("D_003B26E0", "g1_003B25A8", "0x138") G1_ALIAS("Fiona_StateCmdWalkSpot_ptmf", "g1_003B25A8", "0x140") G1_ALIAS("D_003B26F0", "g1_003B25A8", "0x148") G1_ALIAS("Fiona_StateCmdPathSpot_ptmf", "g1_003B25A8", "0x150") G1_ALIAS("D_003B2700", "g1_003B25A8", "0x158") G1_ALIAS("Fiona_StateCmdPathTo_ptmf", "g1_003B25A8", "0x160") G1_ALIAS("D_003B2710", "g1_003B25A8", "0x168") G1_ALIAS("Fiona_StateStep_ptmf", "g1_003B25A8", "0x170") G1_ALIAS("D_003B2720", "g1_003B25A8", "0x178") G1_ALIAS("Fiona_StateStep_ptmf2", "g1_003B25A8", "0x180") G1_ALIAS("D_003B2730", "g1_003B25A8", "0x188") G1_ALIAS("Fiona_StateStep_ptmf3", "g1_003B25A8", "0x190") G1_ALIAS("D_003B2740", "g1_003B25A8", "0x198") G1_ALIAS("Fiona_StateStep_ptmf4", "g1_003B25A8", "0x1A0") G1_ALIAS("D_003B2750", "g1_003B25A8", "0x1A8") G1_ALIAS("Fiona_StateTurnOnSpot_ptmf", "g1_003B25A8", "0x1B0") G1_ALIAS("D_003B2760", "g1_003B25A8", "0x1B8") G1_ALIAS("Fiona_StateTurnOnSpot_ptmf2", "g1_003B25A8", "0x1C0") G1_ALIAS("D_003B2770", "g1_003B25A8", "0x1C8") G1_ALIAS("Fiona_StateCmdWalkAnim_ptmf", "g1_003B25A8", "0x1D0") G1_ALIAS("D_003B2780", "g1_003B25A8", "0x1D8") G1_ALIAS("Fiona_StatePanicFall_ptmf", "g1_003B25A8", "0x1E0") G1_ALIAS("D_003B2790", "g1_003B25A8", "0x1E8") G1_ALIAS("Fiona_StateIdleAtEvent2_ptmf", "g1_003B25A8", "0x1F0") G1_ALIAS("D_003B27A0", "g1_003B25A8", "0x1F8") G1_ALIAS("Fiona_StatePanicAttack_ptmf", "g1_003B25A8", "0x200") G1_ALIAS("D_003B27B0", "g1_003B25A8", "0x208") G1_ALIAS("Fiona_StateActionOver_ptmf", "g1_003B25A8", "0x210") G1_ALIAS("D_003B27C0", "g1_003B25A8", "0x218") G1_ALIAS("Fiona_StateTurnStick_ptmf", "g1_003B25A8", "0x220") G1_ALIAS("D_003B27D0", "g1_003B25A8", "0x228") G1_ALIAS("Fiona_StateSpecialEntry_ptmf", "g1_003B25A8", "0x230") G1_ALIAS("D_003B27E0", "g1_003B25A8", "0x238") G1_ALIAS("Fiona_StateThrown_ptmf", "g1_003B25A8", "0x240") G1_ALIAS("D_003B27F0", "g1_003B25A8", "0x248") G1_ALIAS("Fiona_StatePush_ptmf", "g1_003B25A8", "0x250") G1_ALIAS("D_003B2800", "g1_003B25A8", "0x258") G1_ALIAS("Fiona_StatePushLetGo_ptmf", "g1_003B25A8", "0x260") G1_ALIAS("D_003B2810", "g1_003B25A8", "0x268") G1_ALIAS("Fiona_StatePushStep_ptmf", "g1_003B25A8", "0x270") G1_ALIAS("D_003B2820", "g1_003B25A8", "0x278") G1_ALIAS("Fiona_StatePushLetGo_ptmf2", "g1_003B25A8", "0x280") G1_ALIAS("D_003B2830", "g1_003B25A8", "0x288") G1_ALIAS("Fiona_StatePushLetGo_ptmf3", "g1_003B25A8", "0x290") G1_ALIAS("D_003B2840", "g1_003B25A8", "0x298") G1_ALIAS("Fiona_StateLadderWalk_ptmf", "g1_003B25A8", "0x2A0") G1_ALIAS("D_003B2850", "g1_003B25A8", "0x2A8") G1_ALIAS("Fiona_StateOnLadder_ptmf", "g1_003B25A8", "0x2B0") G1_ALIAS("D_003B2860", "g1_003B25A8", "0x2B8") G1_ALIAS("Fiona_StateCaughtCrawling_ptmf", "g1_003B25A8", "0x2C0") G1_ALIAS("D_003B2870", "g1_003B25A8", "0x2C8") G1_ALIAS("Fiona_StateLadderOff_ptmf", "g1_003B25A8", "0x2D0") G1_ALIAS("D_003B2880", "g1_003B25A8", "0x2D8") G1_ALIAS("Fiona_StateLadderOff_ptmf2", "g1_003B25A8", "0x2E0") G1_ALIAS("D_003B2890", "g1_003B25A8", "0x2E8") G1_ALIAS("Fiona_StateLadderOff_ptmf3", "g1_003B25A8", "0x2F0") G1_ALIAS("D_003B28A0", "g1_003B25A8", "0x2F8") G1_ALIAS("Fiona_StateLadderOff_ptmf4", "g1_003B25A8", "0x300") G1_ALIAS("D_003B28B0", "g1_003B25A8", "0x308") G1_ALIAS("Fiona_StateLockedSlow_ptmf", "g1_003B25A8", "0x310") G1_ALIAS("D_003B28C0", "g1_003B25A8", "0x318") G1_ALIAS("Fiona_StateLockedSpot_ptmf", "g1_003B25A8", "0x320") G1_ALIAS("D_003B28D0", "g1_003B25A8", "0x328") G1_ALIAS("Fiona_StateLockedQuick_ptmf", "g1_003B25A8", "0x330") G1_ALIAS("D_003B28E0", "g1_003B25A8", "0x338") G1_ALIAS("Fiona_StateDoorWalk_ptmf", "g1_003B25A8", "0x340") G1_ALIAS("D_003B28F0", "g1_003B25A8", "0x348") G1_ALIAS("Fiona_StateDoorAnim_ptmf", "g1_003B25A8", "0x350") G1_ALIAS("D_003B2900", "g1_003B25A8", "0x358") G1_ALIAS("Fiona_StateDoorStepOut_ptmf", "g1_003B25A8", "0x360") G1_ALIAS("D_003B2910", "g1_003B25A8", "0x368") G1_ALIAS("Fiona_StateLockedTry_ptmf", "g1_003B25A8", "0x370") G1_ALIAS("D_003B2920", "g1_003B25A8", "0x378") G1_ALIAS("Fiona_StateIdleAtEvent2_ptmf2", "g1_003B25A8", "0x380") G1_ALIAS("D_003B2930", "g1_003B25A8", "0x388") G1_ALIAS("Fiona_StateIdleAtEvent2_ptmf3", "g1_003B25A8", "0x390") G1_ALIAS("D_003B2940", "g1_003B25A8", "0x398") G1_ALIAS("Fiona_StateScriptDoorWalk_ptmf", "g1_003B25A8", "0x3A0") G1_ALIAS("D_003B2950", "g1_003B25A8", "0x3A8") G1_ALIAS("Fiona_StateScriptDoorOpened_ptmf", "g1_003B25A8", "0x3B0") G1_ALIAS("D_003B2960", "g1_003B25A8", "0x3B8") G1_ALIAS("Fiona_StateStrike_ptmf", "g1_003B25A8", "0x3C0") G1_ALIAS("D_003B2970", "g1_003B25A8", "0x3C8") G1_ALIAS("Fiona_StateShove_ptmf", "g1_003B25A8", "0x3D0") G1_ALIAS("D_003B2980", "g1_003B25A8", "0x3D8") G1_ALIAS("Fiona_StateKick_ptmf", "g1_003B25A8", "0x3E0") G1_ALIAS("D_003B2990", "g1_003B25A8", "0x3E8") G1_ALIAS("Fiona_StateAfterCaught_ptmf", "g1_003B25A8", "0x3F0") G1_ALIAS("D_003B29A0", "g1_003B25A8", "0x3F8") G1_ALIAS("Fiona_StateCrawlDrop_ptmf", "g1_003B25A8", "0x400") G1_ALIAS("D_003B29B0", "g1_003B25A8", "0x408") G1_ALIAS("Fiona_StateCrawlPull_ptmf", "g1_003B25A8", "0x410") G1_ALIAS("D_003B29C0", "g1_003B25A8", "0x418") G1_ALIAS("Fiona_StateCrawlFall_ptmf", "g1_003B25A8", "0x420") G1_ALIAS("D_003B29D0", "g1_003B25A8", "0x428") G1_ALIAS("Fiona_StateCrawlDown_ptmf", "g1_003B25A8", "0x430") G1_ALIAS("D_003B29E0", "g1_003B25A8", "0x438") G1_ALIAS("Fiona_StateCrawlPull_ptmf2", "g1_003B25A8", "0x440") G1_ALIAS("D_003B29F0", "g1_003B25A8", "0x448") G1_ALIAS("Fiona_StateLedHand_ptmf", "g1_003B25A8", "0x450") G1_ALIAS("D_003B2A00", "g1_003B25A8", "0x458") G1_ALIAS("Fiona_StateHandSpot_ptmf", "g1_003B25A8", "0x460") G1_ALIAS("D_003B2A10", "g1_003B25A8", "0x468") G1_ALIAS("Fiona_StatePulledFree_ptmf", "g1_003B25A8", "0x470") G1_ALIAS("D_003B2A20", "g1_003B25A8", "0x478") G1_ALIAS("Fiona_StateDragged_ptmf", "g1_003B25A8", "0x480") G1_ALIAS("D_003B2A30", "g1_003B25A8", "0x488") G1_ALIAS("Fiona_StatePulledFree_ptmf2", "g1_003B25A8", "0x490") G1_ALIAS("D_003B2A40", "g1_003B25A8", "0x498") G1_ALIAS("Fiona_StatePulledFree_ptmf3", "g1_003B25A8", "0x4A0") G1_ALIAS("D_003B2A50", "g1_003B25A8", "0x4A8") G1_ALIAS("Fiona_StatePulledFree_ptmf4", "g1_003B25A8", "0x4B0") G1_ALIAS("D_003B2A60", "g1_003B25A8", "0x4B8") G1_ALIAS("Fiona_StateDraggedOff_ptmf", "g1_003B25A8", "0x4C0") G1_ALIAS("D_003B2A70", "g1_003B25A8", "0x4C8") G1_ALIAS("Fiona_StatePulledFree_ptmf5", "g1_003B25A8", "0x4D0") G1_ALIAS("D_003B2A80", "g1_003B25A8", "0x4D8") G1_ALIAS("Fiona_StatePulledFree_ptmf6", "g1_003B25A8", "0x4E0") G1_ALIAS("D_003B2A90", "g1_003B25A8", "0x4E8") G1_ALIAS("Fiona_StateLedAway_ptmf", "g1_003B25A8", "0x4F0") G1_ALIAS("D_003B2AA0", "g1_003B25A8", "0x4F8") G1_ALIAS("Fiona_StateLedSpot_ptmf", "g1_003B25A8", "0x500") G1_ALIAS("D_003B2AB0", "g1_003B25A8", "0x508") G1_ALIAS("Fiona_StateAfterThrow_ptmf", "g1_003B25A8", "0x510") G1_ALIAS("D_003B2AC0", "g1_003B25A8", "0x518") G1_ALIAS("Fiona_StateGetUp_ptmf", "g1_003B25A8", "0x520") G1_ALIAS("D_003B2AD0", "g1_003B25A8", "0x528") G1_ALIAS("Fiona_StateSpecialEntry_ptmf2", "g1_003B25A8", "0x530") G1_ALIAS("D_003B2AE0", "g1_003B25A8", "0x538") G1_ALIAS("Fiona_StateGetUp_ptmf2", "g1_003B25A8", "0x540") G1_ALIAS("D_003B2AF0", "g1_003B25A8", "0x548") G1_ALIAS("Fiona_StateSpecialEntry_ptmf3", "g1_003B25A8", "0x550") G1_ALIAS("D_003B2B00", "g1_003B25A8", "0x558") G1_ALIAS("Fiona_StateAfterThrow_ptmf2", "g1_003B25A8", "0x560") G1_ALIAS("D_003B2B10", "g1_003B25A8", "0x568") G1_ALIAS("Fiona_StateAfterKnockDown_ptmf", "g1_003B25A8", "0x570") G1_ALIAS("D_003B2B20", "g1_003B25A8", "0x578") G1_ALIAS("Fiona_StateFallen_ptmf", "g1_003B25A8", "0x580") G1_ALIAS("D_003B2B30", "g1_003B25A8", "0x588") G1_ALIAS("Fiona_StateSpecialEntry_ptmf4", "g1_003B25A8", "0x590") G1_ALIAS("D_003B2B40", "g1_003B25A8", "0x598") G1_ALIAS("Fiona_StateHeld_ptmf", "g1_003B25A8", "0x5A0") G1_ALIAS("D_003B2B50", "g1_003B25A8", "0x5A8") G1_ALIAS("Fiona_StateLookEnd_ptmf", "g1_003B25A8", "0x5B0") G1_ALIAS("D_003B2B60", "g1_003B25A8", "0x5B8") G1_ALIAS("Fiona_StateLookEnd_ptmf2", "g1_003B25A8", "0x5C0") G1_ALIAS("D_003B2B70", "g1_003B25A8", "0x5C8") G1_ALIAS("Fiona_StateWalkThenGesture_ptmf", "g1_003B25A8", "0x5D0") G1_ALIAS("D_003B2B80", "g1_003B25A8", "0x5D8") G1_ALIAS("Fiona_StateLookEnd_ptmf3", "g1_003B25A8", "0x5E0") G1_ALIAS("D_003B2B90", "g1_003B25A8", "0x5E8") G1_ALIAS("Fiona_StateLookEnd_ptmf4", "g1_003B25A8", "0x5F0") G1_ALIAS("D_003B2BA0", "g1_003B25A8", "0x5F8") G1_ALIAS("Fiona_StateLookEnd_ptmf5", "g1_003B25A8", "0x600") G1_ALIAS("D_003B2BB0", "g1_003B25A8", "0x608") G1_ALIAS("Fiona_StateHeldStart_ptmf", "g1_003B25A8", "0x610") G1_ALIAS("D_003B2BC0", "g1_003B25A8", "0x618") G1_ALIAS("Fiona_StateHeldStart_ptmf2", "g1_003B25A8", "0x620") G1_ALIAS("D_003B2BD0", "g1_003B25A8", "0x628") G1_ALIAS("Fiona_StateActionOver_ptmf2", "g1_003B25A8", "0x630") G1_ALIAS("D_003B2BE0", "g1_003B25A8", "0x638") G1_ALIAS("Fiona_StateHeldStart_ptmf3", "g1_003B25A8", "0x640") G1_ALIAS("D_003B2BF0", "g1_003B25A8", "0x648") G1_ALIAS("Fiona_StateActionOver_ptmf3", "g1_003B25A8", "0x650") G1_ALIAS("D_003B2C00", "g1_003B25A8", "0x658") G1_ALIAS("Fiona_StateLookEnd_ptmf6", "g1_003B25A8", "0x660") G1_ALIAS("D_003B2C10", "g1_003B25A8", "0x668") G1_ALIAS("Fiona_StateLookAround_ptmf", "g1_003B25A8", "0x670") G1_ALIAS("D_003B2C20", "g1_003B25A8", "0x678") G1_ALIAS("Fiona_StateActionOver_ptmf4", "g1_003B25A8", "0x680") G1_ALIAS("D_003B2C30", "g1_003B25A8", "0x688") G1_ALIAS("Fiona_StateActionOver_ptmf5", "g1_003B25A8", "0x690") G1_ALIAS("D_003B2C40", "g1_003B25A8", "0x698") G1_ALIAS("Fiona_StateHeldWalk_ptmf", "g1_003B25A8", "0x6A0") G1_ALIAS("D_003B2C50", "g1_003B25A8", "0x6A8") G1_ALIAS("Fiona_StateHeldStopped_ptmf", "g1_003B25A8", "0x6B0") G1_ALIAS("D_003B2C60", "g1_003B25A8", "0x6B8") G1_ALIAS("Fiona_StateWaitHewie_ptmf", "g1_003B25A8", "0x6C0") G1_ALIAS("D_003B2C70", "g1_003B25A8", "0x6C8") G1_ALIAS("Fiona_StateHeldGesture_ptmf", "g1_003B25A8", "0x6D0") G1_ALIAS("D_003B2C80", "g1_003B25A8", "0x6D8") G1_ALIAS("Fiona_StateHeldGesture_ptmf2", "g1_003B25A8", "0x6E0") G1_ALIAS("D_003B2C90", "g1_003B25A8", "0x6E8") G1_ALIAS("Fiona_StateHeldGesture_ptmf3", "g1_003B25A8", "0x6F0") G1_ALIAS("D_003B2CA0", "g1_003B25A8", "0x6F8") G1_ALIAS("Fiona_StateWaitHewie_ptmf2", "g1_003B25A8", "0x700") G1_ALIAS("D_003B2CB0", "g1_003B25A8", "0x708") G1_ALIAS("Fiona_StateHeldCommand_ptmf", "g1_003B25A8", "0x710") G1_ALIAS("D_003B2CC0", "g1_003B25A8", "0x718") G1_ALIAS("Fiona_StateGestureLeadIn_ptmf", "g1_003B25A8", "0x720") G1_ALIAS("D_003B2CD0", "g1_003B25A8", "0x728") G1_ALIAS("Fiona_StateGesture_ptmf", "g1_003B25A8", "0x730") G1_ALIAS("D_003B2CE0", "g1_003B25A8", "0x738") G1_ALIAS("Fiona_StateGestureEnd_ptmf", "g1_003B25A8", "0x740") G1_ALIAS("D_003B2CF0", "g1_003B25A8", "0x748") G1_ALIAS("Fiona_StateTurnUntilDone_ptmf", "g1_003B25A8", "0x750") G1_ALIAS("D_003B2D00", "g1_003B25A8", "0x758") G1_ALIAS("Fiona_StateIdleAtEvent_ptmf", "g1_003B25A8", "0x760") G1_ALIAS("D_003B2D10", "g1_003B25A8", "0x768") G1_ALIAS("Fiona_StateLetGo_ptmf", "g1_003B25A8", "0x770") G1_ALIAS("D_003B2D20", "g1_003B25A8", "0x778") G1_ALIAS("Fiona_StateFleeTurn_ptmf", "g1_003B25A8", "0x780") G1_ALIAS("D_003B2D30", "g1_003B25A8", "0x788") G1_ALIAS("Fiona_StateStruggle_ptmf", "g1_003B25A8", "0x790") G1_ALIAS("D_003B2D40", "g1_003B25A8", "0x798") G1_ALIAS("Fiona_StateAfterFall_ptmf", "g1_003B25A8", "0x7A0") G1_ALIAS("D_003B2D50", "g1_003B25A8", "0x7A8") G1_ALIAS("Fiona_StateOnGround_ptmf", "g1_003B25A8", "0x7B0") G1_ALIAS("D_003B2D60", "g1_003B25A8", "0x7B8") G1_ALIAS("Fiona_StateCmdPathTurn_ptmf", "g1_003B25A8", "0x7C0") G1_ALIAS("D_003B2D70", "g1_003B25A8", "0x7C8") G1_ALIAS("Fiona_StateCmdAnimDone_ptmf", "g1_003B25A8", "0x7D0") G1_ALIAS("D_003B2D80", "g1_003B25A8", "0x7D8") G1_ALIAS("Fiona_StateCmdTurn_ptmf", "g1_003B25A8", "0x7E0") G1_ALIAS("D_003B2D90", "g1_003B25A8", "0x7E8") G1_ALIAS("Fiona_StateCmdAnimDone_ptmf2", "g1_003B25A8", "0x7F0") G1_ALIAS("D_003B2DA0", "g1_003B25A8", "0x7F8") G1_ALIAS("Fiona_StateCmdPath_ptmf", "g1_003B25A8", "0x800") G1_ALIAS("D_003B2DB0", "g1_003B25A8", "0x808") G1_ALIAS("Fiona_StateCmdToSpot_ptmf", "g1_003B25A8", "0x810") G1_ALIAS("D_003B2DC0", "g1_003B25A8", "0x818") G1_ALIAS("Fiona_StateTurnStanding_ptmf", "g1_003B25A8", "0x820") G1_ALIAS("D_003B2DD0", "g1_003B25A8", "0x828") G1_ALIAS("Fiona_StateThrown_ptmf2", "g1_003B25A8", "0x830") G1_ALIAS("D_003B2DE0", "g1_003B25A8", "0x838") G1_ALIAS("Fiona_StateKnockedDown_ptmf", "g1_003B25A8", "0x840") G1_ALIAS("D_003B2DF0", "g1_003B25A8", "0x848") G1_ALIAS("Fiona_StateCaught_ptmf3", "g1_003B25A8", "0x850") G1_ALIAS("D_003B2E00", "g1_003B25A8", "0x858") G1_ALIAS("Fiona_StateGrabbed_ptmf", "g1_003B25A8", "0x860") G1_ALIAS("D_003B2E10", "g1_003B25A8", "0x868") G1_ALIAS("Fiona_StateCaughtCrawling_ptmf2", "g1_003B25A8", "0x870") G1_ALIAS("D_003B2E20", "g1_003B25A8", "0x878") G1_ALIAS("Fiona_StateLedByHand_ptmf", "g1_003B25A8", "0x880") G1_ALIAS("D_003B2E30", "g1_003B25A8", "0x888") G1_ALIAS("Fiona_StateLedWalking_ptmf", "g1_003B25A8", "0x890") G1_ALIAS("D_003B2E40", "g1_003B25A8", "0x898") G1_ALIAS("Fiona_StateThrown_ptmf3", "g1_003B25A8", "0x8A0") G1_ALIAS("D_003B2E50", "g1_003B25A8", "0x8A8") G1_ALIAS("Fiona_StateHitByDoor_ptmf", "g1_003B25A8", "0x8B0") G1_ALIAS("D_003B2E60", "g1_003B25A8", "0x8B8") G1_ALIAS("Fiona_StateKnockedDown_ptmf2", "g1_003B25A8", "0x8C0") G1_ALIAS("D_003B2E70", "g1_003B25A8", "0x8C8") G1_ALIAS("Fiona_StateHold_ptmf", "g1_003B25A8", "0x8D0") G1_ALIAS("D_003B2E80", "g1_003B25A8", "0x8D8") G1_ALIAS("Fiona_StateFall13_ptmf", "g1_003B25A8", "0x8E0") G1_ALIAS("D_003B2E90", "g1_003B25A8", "0x8E8") G1_ALIAS("Fiona_StateDoorStart_ptmf4", "g1_003B25A8", "0x8F0") G1_ALIAS("D_003B2EA0", "g1_003B25A8", "0x8F8") G1_ALIAS("PathPlan_StepBreadth_ptmf", "g1_003B25A8", "0x900") G1_ALIAS("D_003B2EB0", "g1_003B25A8", "0x908") G1_ALIAS("D_003B2EB8", "g1_003B25A8", "0x910") G1_ALIAS("D_003B2EBC", "g1_003B25A8", "0x914") G1_ALIAS("D_003B2EC0", "g1_003B25A8", "0x918") G1_ALIAS("D_003B2EC8", "g1_003B25A8", "0x920") G1_ALIAS("D_003B2ECC", "g1_003B25A8", "0x924") G1_ALIAS("D_003B2ED0", "g1_003B25A8", "0x928") G1_ALIAS("D_003B2ED8", "g1_003B25A8", "0x930") G1_ALIAS("D_003B2EDC", "g1_003B25A8", "0x934") G1_ALIAS("D_003B2EE0", "g1_003B25A8", "0x938") G1_ALIAS("D_003B2EE8", "g1_003B25A8", "0x940") G1_ALIAS("D_003B2EEC", "g1_003B25A8", "0x944") G1_ALIAS("D_003B2EF0", "g1_003B25A8", "0x948") G1_ALIAS("D_003B2EF8", "g1_003B25A8", "0x950") G1_ALIAS("D_003B2EFC", "g1_003B25A8", "0x954") G1_ALIAS("D_003B2F00", "g1_003B25A8", "0x958") G1_ALIAS("D_003B2F08", "g1_003B25A8", "0x960") G1_ALIAS("D_003B2F0C", "g1_003B25A8", "0x964") G1_ALIAS("D_003B2F10", "g1_003B25A8", "0x968") G1_ALIAS("D_003B2F18", "g1_003B25A8", "0x970") G1_ALIAS("D_003B2F1C", "g1_003B25A8", "0x974") G1_ALIAS("D_003B2F20", "g1_003B25A8", "0x978") G1_ALIAS("D_003B2F28", "g1_003B25A8", "0x980") G1_ALIAS("D_003B2F2C", "g1_003B25A8", "0x984") G1_ALIAS("D_003B2F30", "g1_003B25A8", "0x988"));

/* 0x003B2F68: Palette_FlatLow_ptmf, D_003B2F70, Palette_FlatHigh_ptmf, D_003B2F7C, D_003B2F80, Palette_Colour_ptmf ... */
void *g1_003B2F68[54] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Palette_FlatLow, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Palette_FlatHigh, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Palette_Colour, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Palette_RandomGrey, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Palette_Grey, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Palette_Threshold, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Palette_Alpha11, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)func_00225770, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)func_00225860, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)func_00225C40, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)func_00225950, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)func_00225F30, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)func_00226220, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Palette_FlatLow_ptmf", "g1_003B2F68", "0x0") G1_ALIAS("D_003B2F70", "g1_003B2F68", "0x8") G1_ALIAS("Palette_FlatHigh_ptmf", "g1_003B2F68", "0x10") G1_ALIAS("D_003B2F7C", "g1_003B2F68", "0x14") G1_ALIAS("D_003B2F80", "g1_003B2F68", "0x18") G1_ALIAS("Palette_Colour_ptmf", "g1_003B2F68", "0x20") G1_ALIAS("D_003B2F8C", "g1_003B2F68", "0x24") G1_ALIAS("D_003B2F90", "g1_003B2F68", "0x28") G1_ALIAS("Palette_RandomGrey_ptmf", "g1_003B2F68", "0x30") G1_ALIAS("D_003B2F9C", "g1_003B2F68", "0x34") G1_ALIAS("D_003B2FA0", "g1_003B2F68", "0x38") G1_ALIAS("Palette_Grey_ptmf", "g1_003B2F68", "0x40") G1_ALIAS("D_003B2FAC", "g1_003B2F68", "0x44") G1_ALIAS("D_003B2FB0", "g1_003B2F68", "0x48") G1_ALIAS("Palette_Threshold_ptmf", "g1_003B2F68", "0x50") G1_ALIAS("D_003B2FBC", "g1_003B2F68", "0x54") G1_ALIAS("D_003B2FC0", "g1_003B2F68", "0x58") G1_ALIAS("Palette_Alpha11_ptmf", "g1_003B2F68", "0x60") G1_ALIAS("D_003B2FCC", "g1_003B2F68", "0x64") G1_ALIAS("D_003B2FD0", "g1_003B2F68", "0x68") G1_ALIAS("D_003B2FD8", "g1_003B2F68", "0x70") G1_ALIAS("D_003B2FE0", "g1_003B2F68", "0x78") G1_ALIAS("D_003B2FE8", "g1_003B2F68", "0x80") G1_ALIAS("D_003B2FF0", "g1_003B2F68", "0x88") G1_ALIAS("D_003B2FF8", "g1_003B2F68", "0x90") G1_ALIAS("D_003B3000", "g1_003B2F68", "0x98") G1_ALIAS("D_003B3008", "g1_003B2F68", "0xA0") G1_ALIAS("D_003B3010", "g1_003B2F68", "0xA8") G1_ALIAS("D_003B3018", "g1_003B2F68", "0xB0") G1_ALIAS("D_003B3020", "g1_003B2F68", "0xB8") G1_ALIAS("D_003B3028", "g1_003B2F68", "0xC0") G1_ALIAS("D_003B3030", "g1_003B2F68", "0xC8"));

/* 0x003CB750: D_003CB750, D_003CB758 */
void *g1_003CB750[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0x00000000u, (void *)D_004543E0, (void *)0x00000000u,
};
__asm__(G1_ALIAS("D_003CB750", "g1_003CB750", "0x0") G1_ALIAS("D_003CB758", "g1_003CB750", "0x8"));

/* 0x003D5C68: Gesture_Back_ptmf, D_003D5C70, D_003D5C78, D_003D5C7C, D_003D5C80, D_003D5C88 ... */
void *g1_003D5C68[22] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Gesture_Back, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Gesture_Forward, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Gesture_R3, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Gesture_Right, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Gesture_Left, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Gesture_Back_ptmf", "g1_003D5C68", "0x0") G1_ALIAS("D_003D5C70", "g1_003D5C68", "0x8") G1_ALIAS("D_003D5C78", "g1_003D5C68", "0x10") G1_ALIAS("D_003D5C7C", "g1_003D5C68", "0x14") G1_ALIAS("D_003D5C80", "g1_003D5C68", "0x18") G1_ALIAS("D_003D5C88", "g1_003D5C68", "0x20") G1_ALIAS("D_003D5C8C", "g1_003D5C68", "0x24") G1_ALIAS("D_003D5C90", "g1_003D5C68", "0x28") G1_ALIAS("D_003D5C98", "g1_003D5C68", "0x30") G1_ALIAS("D_003D5C9C", "g1_003D5C68", "0x34") G1_ALIAS("D_003D5CA0", "g1_003D5C68", "0x38") G1_ALIAS("D_003D5CA8", "g1_003D5C68", "0x40") G1_ALIAS("D_003D5CAC", "g1_003D5C68", "0x44") G1_ALIAS("D_003D5CB0", "g1_003D5C68", "0x48"));

/* 0x003D8920: Game_Init_ptmf */
void *g1_003D8920[4] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Game_Init, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Game_Init_ptmf", "g1_003D8920", "0x0"));

/* 0x003E5240: D_003E5240, D_003E5244, D_003E5248, CamDirector_Remember_ptmf, D_003E5254, D_003E5258 */
void *g1_003E5240[8] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CamDirector_ModeNormal, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CamDirector_Remember, (void *)0x00000000u,
};
__asm__(G1_ALIAS("D_003E5240", "g1_003E5240", "0x0") G1_ALIAS("D_003E5244", "g1_003E5240", "0x4") G1_ALIAS("D_003E5248", "g1_003E5240", "0x8") G1_ALIAS("CamDirector_Remember_ptmf", "g1_003E5240", "0x10") G1_ALIAS("D_003E5254", "g1_003E5240", "0x14") G1_ALIAS("D_003E5258", "g1_003E5240", "0x18"));

/* 0x003EC140: Pursuer_StateStand_ptmf, D_003EC148 */
void *g1_003EC140[4] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateStand, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateStand_ptmf", "g1_003EC140", "0x0") G1_ALIAS("D_003EC148", "g1_003EC140", "0x8"));

/* 0x003EC160: Pursuer_StateWalkThenAnim_ptmf, D_003EC164, D_003EC168 */
void *g1_003EC160[4] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateWalkThenAnim, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateWalkThenAnim_ptmf", "g1_003EC160", "0x0") G1_ALIAS("D_003EC164", "g1_003EC160", "0x4") G1_ALIAS("D_003EC168", "g1_003EC160", "0x8"));

/* 0x003EC270: Pursuer_AttackNextStep_ptmf2, D_003EC274, D_003EC278, Pursuer_StateFaceFiona_ptmf, D_003EC284, D_003EC288 ... */
void *g1_003EC270[36] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_AttackNextStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateFaceFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateWalkOn, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateLookAround, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateWalkGesture, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateWalkGesture, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateWalkGesture, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateSidestepRoom, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateSidestepRoom, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_AttackNextStep_ptmf2", "g1_003EC270", "0x0") G1_ALIAS("D_003EC274", "g1_003EC270", "0x4") G1_ALIAS("D_003EC278", "g1_003EC270", "0x8") G1_ALIAS("Pursuer_StateFaceFiona_ptmf", "g1_003EC270", "0x10") G1_ALIAS("D_003EC284", "g1_003EC270", "0x14") G1_ALIAS("D_003EC288", "g1_003EC270", "0x18") G1_ALIAS("Pursuer_StateWalkOn_ptmf", "g1_003EC270", "0x20") G1_ALIAS("D_003EC294", "g1_003EC270", "0x24") G1_ALIAS("D_003EC298", "g1_003EC270", "0x28") G1_ALIAS("Pursuer_StateLookAround_ptmf", "g1_003EC270", "0x30") G1_ALIAS("D_003EC2A4", "g1_003EC270", "0x34") G1_ALIAS("D_003EC2A8", "g1_003EC270", "0x38") G1_ALIAS("Pursuer_StateWalkGesture_ptmf", "g1_003EC270", "0x40") G1_ALIAS("D_003EC2B4", "g1_003EC270", "0x44") G1_ALIAS("D_003EC2B8", "g1_003EC270", "0x48") G1_ALIAS("Pursuer_StateWalkGesture_ptmf2", "g1_003EC270", "0x50") G1_ALIAS("D_003EC2C4", "g1_003EC270", "0x54") G1_ALIAS("D_003EC2C8", "g1_003EC270", "0x58") G1_ALIAS("Pursuer_StateWalkGesture_ptmf3", "g1_003EC270", "0x60") G1_ALIAS("D_003EC2D4", "g1_003EC270", "0x64") G1_ALIAS("D_003EC2D8", "g1_003EC270", "0x68") G1_ALIAS("Pursuer_StateSidestepRoom_ptmf", "g1_003EC270", "0x70") G1_ALIAS("D_003EC2E4", "g1_003EC270", "0x74") G1_ALIAS("D_003EC2E8", "g1_003EC270", "0x78") G1_ALIAS("Pursuer_StateSidestepRoom_ptmf2", "g1_003EC270", "0x80") G1_ALIAS("D_003EC2F4", "g1_003EC270", "0x84") G1_ALIAS("D_003EC2F8", "g1_003EC270", "0x88"));

/* 0x003EC310: Pursuer_StateWalkThen404_ptmf, D_003EC314, D_003EC318, Pursuer_StateFlinch_ptmf, D_003EC324, D_003EC328 ... */
void *g1_003EC310[32] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateWalkThen404, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateFlinch, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateKnockedDown, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateHurt, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateHitAtDoor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateDoorAhead, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateHewieBites, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateSpecialAnim, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateWalkThen404_ptmf", "g1_003EC310", "0x0") G1_ALIAS("D_003EC314", "g1_003EC310", "0x4") G1_ALIAS("D_003EC318", "g1_003EC310", "0x8") G1_ALIAS("Pursuer_StateFlinch_ptmf", "g1_003EC310", "0x10") G1_ALIAS("D_003EC324", "g1_003EC310", "0x14") G1_ALIAS("D_003EC328", "g1_003EC310", "0x18") G1_ALIAS("Pursuer_StateKnockedDown_ptmf", "g1_003EC310", "0x20") G1_ALIAS("D_003EC334", "g1_003EC310", "0x24") G1_ALIAS("D_003EC338", "g1_003EC310", "0x28") G1_ALIAS("Pursuer_StateHurt_ptmf", "g1_003EC310", "0x30") G1_ALIAS("D_003EC344", "g1_003EC310", "0x34") G1_ALIAS("D_003EC348", "g1_003EC310", "0x38") G1_ALIAS("Pursuer_StateHitAtDoor_ptmf", "g1_003EC310", "0x40") G1_ALIAS("D_003EC354", "g1_003EC310", "0x44") G1_ALIAS("D_003EC358", "g1_003EC310", "0x48") G1_ALIAS("Pursuer_StateDoorAhead_ptmf", "g1_003EC310", "0x50") G1_ALIAS("D_003EC364", "g1_003EC310", "0x54") G1_ALIAS("D_003EC368", "g1_003EC310", "0x58") G1_ALIAS("Pursuer_StateHewieBites_ptmf", "g1_003EC310", "0x60") G1_ALIAS("D_003EC374", "g1_003EC310", "0x64") G1_ALIAS("D_003EC378", "g1_003EC310", "0x68") G1_ALIAS("Pursuer_StateSpecialAnim_ptmf", "g1_003EC310", "0x70") G1_ALIAS("D_003EC384", "g1_003EC310", "0x74") G1_ALIAS("D_003EC388", "g1_003EC310", "0x78"));

/* 0x003EC930: Pursuer_StateRunThenNext_ptmf, D_003EC938 */
void *g1_003EC930[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf", "g1_003EC930", "0x0") G1_ALIAS("D_003EC938", "g1_003EC930", "0x8"));

/* 0x003ECA70: Pursuer_StateHitReact_ptmf, D_003ECA78 */
void *g1_003ECA70[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateHitReact, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateHitReact_ptmf", "g1_003ECA70", "0x0") G1_ALIAS("D_003ECA78", "g1_003ECA70", "0x8"));

/* 0x003ECA90: Pursuer_StateRunThenNext_ptmf2, D_003ECA98, Pursuer_StateRunThenNext_ptmf3, D_003ECAA8, Pursuer_StateRunThenNext_ptmf4, D_003ECAB8 ... */
void *g1_003ECA90[56] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateArrive, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateArrive, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunToTri, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunToTri, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRootStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRootStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRootStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRootStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateTurnThenWait, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateTurnThenWait, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf2", "g1_003ECA90", "0x0") G1_ALIAS("D_003ECA98", "g1_003ECA90", "0x8") G1_ALIAS("Pursuer_StateRunThenNext_ptmf3", "g1_003ECA90", "0x10") G1_ALIAS("D_003ECAA8", "g1_003ECA90", "0x18") G1_ALIAS("Pursuer_StateRunThenNext_ptmf4", "g1_003ECA90", "0x20") G1_ALIAS("D_003ECAB8", "g1_003ECA90", "0x28") G1_ALIAS("Pursuer_StateArrive_ptmf", "g1_003ECA90", "0x30") G1_ALIAS("D_003ECAC8", "g1_003ECA90", "0x38") G1_ALIAS("Pursuer_StateArrive_ptmf2", "g1_003ECA90", "0x40") G1_ALIAS("D_003ECAD8", "g1_003ECA90", "0x48") G1_ALIAS("Pursuer_StateRunToTri_ptmf", "g1_003ECA90", "0x50") G1_ALIAS("D_003ECAE8", "g1_003ECA90", "0x58") G1_ALIAS("Pursuer_StateRunToTri_ptmf2", "g1_003ECA90", "0x60") G1_ALIAS("D_003ECAF8", "g1_003ECA90", "0x68") G1_ALIAS("Pursuer_StateRootStep_ptmf", "g1_003ECA90", "0x70") G1_ALIAS("D_003ECB08", "g1_003ECA90", "0x78") G1_ALIAS("Pursuer_StateRootStep_ptmf2", "g1_003ECA90", "0x80") G1_ALIAS("D_003ECB18", "g1_003ECA90", "0x88") G1_ALIAS("Pursuer_StateRootStep_ptmf3", "g1_003ECA90", "0x90") G1_ALIAS("D_003ECB28", "g1_003ECA90", "0x98") G1_ALIAS("Pursuer_StateRootStep_ptmf4", "g1_003ECA90", "0xA0") G1_ALIAS("D_003ECB38", "g1_003ECA90", "0xA8") G1_ALIAS("Pursuer_StateTurnThenWait_ptmf", "g1_003ECA90", "0xB0") G1_ALIAS("D_003ECB48", "g1_003ECA90", "0xB8") G1_ALIAS("Pursuer_StateTurnThenWait_ptmf2", "g1_003ECA90", "0xC0") G1_ALIAS("D_003ECB58", "g1_003ECA90", "0xC8") G1_ALIAS("Pursuer_StateRunThenNext_ptmf5", "g1_003ECA90", "0xD0") G1_ALIAS("D_003ECB68", "g1_003ECA90", "0xD8"));

/* 0x003ECD60: Pursuer_StateStopAtEnd_ptmf, D_003ECD68, Pursuer_StateEndStep_ptmf, D_003ECD78, Pursuer_StateEndStep_ptmf2, D_003ECD88 */
void *g1_003ECD60[12] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateStopAtEnd, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateEndStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateEndStep, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateStopAtEnd_ptmf", "g1_003ECD60", "0x0") G1_ALIAS("D_003ECD68", "g1_003ECD60", "0x8") G1_ALIAS("Pursuer_StateEndStep_ptmf", "g1_003ECD60", "0x10") G1_ALIAS("D_003ECD78", "g1_003ECD60", "0x18") G1_ALIAS("Pursuer_StateEndStep_ptmf2", "g1_003ECD60", "0x20") G1_ALIAS("D_003ECD88", "g1_003ECD60", "0x28"));

/* 0x003ECFF0: Pursuer_StateAttackActive_ptmf, D_003ECFF8, Pursuer_StateAttackStep_ptmf, D_003ED008, Pursuer_AttackNextStep_ptmf3, D_003ED018 ... */
void *g1_003ECFF0[72] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateAttackActive, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateAttackStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_AttackNextStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_AttackNextStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateCloseOnFionaB, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRelation2, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateCloseB, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateTauntStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateTaunt, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateTauntRepeat, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateTauntHold, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateTauntHold, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateCloseOnFionaA, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRelation1, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateCloseA, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateGrabStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateStepToEnd, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateEndStep, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateAttackActive_ptmf", "g1_003ECFF0", "0x0") G1_ALIAS("D_003ECFF8", "g1_003ECFF0", "0x8") G1_ALIAS("Pursuer_StateAttackStep_ptmf", "g1_003ECFF0", "0x10") G1_ALIAS("D_003ED008", "g1_003ECFF0", "0x18") G1_ALIAS("Pursuer_AttackNextStep_ptmf3", "g1_003ECFF0", "0x20") G1_ALIAS("D_003ED018", "g1_003ECFF0", "0x28") G1_ALIAS("Pursuer_AttackNextStep_ptmf4", "g1_003ECFF0", "0x30") G1_ALIAS("D_003ED028", "g1_003ECFF0", "0x38") G1_ALIAS("Pursuer_StateCloseOnFionaB_ptmf", "g1_003ECFF0", "0x40") G1_ALIAS("D_003ED038", "g1_003ECFF0", "0x48") G1_ALIAS("Pursuer_StateRelation2_ptmf", "g1_003ECFF0", "0x50") G1_ALIAS("D_003ED048", "g1_003ECFF0", "0x58") G1_ALIAS("Pursuer_StateCloseB_ptmf", "g1_003ECFF0", "0x60") G1_ALIAS("D_003ED058", "g1_003ECFF0", "0x68") G1_ALIAS("Pursuer_StateTauntStart_ptmf", "g1_003ECFF0", "0x70") G1_ALIAS("D_003ED068", "g1_003ECFF0", "0x78") G1_ALIAS("Pursuer_StateTaunt_ptmf", "g1_003ECFF0", "0x80") G1_ALIAS("D_003ED078", "g1_003ECFF0", "0x88") G1_ALIAS("Pursuer_StateTauntRepeat_ptmf", "g1_003ECFF0", "0x90") G1_ALIAS("D_003ED088", "g1_003ECFF0", "0x98") G1_ALIAS("Pursuer_StateTauntHold_ptmf", "g1_003ECFF0", "0xA0") G1_ALIAS("D_003ED098", "g1_003ECFF0", "0xA8") G1_ALIAS("Pursuer_StateTauntHold_ptmf2", "g1_003ECFF0", "0xB0") G1_ALIAS("D_003ED0A8", "g1_003ECFF0", "0xB8") G1_ALIAS("Pursuer_StateCloseOnFionaA_ptmf", "g1_003ECFF0", "0xC0") G1_ALIAS("D_003ED0B8", "g1_003ECFF0", "0xC8") G1_ALIAS("Pursuer_StateRelation1_ptmf", "g1_003ECFF0", "0xD0") G1_ALIAS("D_003ED0C8", "g1_003ECFF0", "0xD8") G1_ALIAS("Pursuer_StateCloseA_ptmf", "g1_003ECFF0", "0xE0") G1_ALIAS("D_003ED0D8", "g1_003ECFF0", "0xE8") G1_ALIAS("Pursuer_StateGrabStart_ptmf", "g1_003ECFF0", "0xF0") G1_ALIAS("D_003ED0E8", "g1_003ECFF0", "0xF8") G1_ALIAS("Pursuer_StateStepToEnd_ptmf", "g1_003ECFF0", "0x100") G1_ALIAS("D_003ED0F8", "g1_003ECFF0", "0x108") G1_ALIAS("Pursuer_StateEndStep_ptmf3", "g1_003ECFF0", "0x110") G1_ALIAS("D_003ED108", "g1_003ECFF0", "0x118"));

/* 0x003ED120: Pursuer_StateEndStep_ptmf4, D_003ED128, Pursuer_StateStepFacing_ptmf, D_003ED138, Pursuer_StateSidestep_ptmf, D_003ED148 ... */
void *g1_003ED120[76] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateEndStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateStepFacing, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateSidestep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateEndStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateDownAndUp, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateHitReact, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateHitReact, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateHitOver, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateHitReaction, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateHitOver, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateDownAndUp, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateHitReact, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateBargedThrough, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateKnockedThrough, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateTurnAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateHewieHolds, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateBackOnFeet, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateBackOnFeet, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateCountKeys, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateEndStep_ptmf4", "g1_003ED120", "0x0") G1_ALIAS("D_003ED128", "g1_003ED120", "0x8") G1_ALIAS("Pursuer_StateStepFacing_ptmf", "g1_003ED120", "0x10") G1_ALIAS("D_003ED138", "g1_003ED120", "0x18") G1_ALIAS("Pursuer_StateSidestep_ptmf", "g1_003ED120", "0x20") G1_ALIAS("D_003ED148", "g1_003ED120", "0x28") G1_ALIAS("Pursuer_StateEndStep_ptmf5", "g1_003ED120", "0x30") G1_ALIAS("D_003ED158", "g1_003ED120", "0x38") G1_ALIAS("Pursuer_StateDownAndUp_ptmf", "g1_003ED120", "0x40") G1_ALIAS("D_003ED168", "g1_003ED120", "0x48") G1_ALIAS("Pursuer_StateHitReact_ptmf2", "g1_003ED120", "0x50") G1_ALIAS("D_003ED178", "g1_003ED120", "0x58") G1_ALIAS("Pursuer_StateHitReact_ptmf3", "g1_003ED120", "0x60") G1_ALIAS("D_003ED188", "g1_003ED120", "0x68") G1_ALIAS("Pursuer_StateHitOver_ptmf", "g1_003ED120", "0x70") G1_ALIAS("D_003ED198", "g1_003ED120", "0x78") G1_ALIAS("Pursuer_StateHitReaction_ptmf", "g1_003ED120", "0x80") G1_ALIAS("D_003ED1A8", "g1_003ED120", "0x88") G1_ALIAS("Pursuer_StateHitOver_ptmf2", "g1_003ED120", "0x90") G1_ALIAS("D_003ED1B8", "g1_003ED120", "0x98") G1_ALIAS("Pursuer_StateDownAndUp_ptmf2", "g1_003ED120", "0xA0") G1_ALIAS("D_003ED1C8", "g1_003ED120", "0xA8") G1_ALIAS("Pursuer_StateHitReact_ptmf4", "g1_003ED120", "0xB0") G1_ALIAS("D_003ED1D8", "g1_003ED120", "0xB8") G1_ALIAS("Pursuer_StateBargedThrough_ptmf", "g1_003ED120", "0xC0") G1_ALIAS("D_003ED1E8", "g1_003ED120", "0xC8") G1_ALIAS("Pursuer_StateKnockedThrough_ptmf", "g1_003ED120", "0xD0") G1_ALIAS("D_003ED1F8", "g1_003ED120", "0xD8") G1_ALIAS("Pursuer_StateTurnAnim_ptmf", "g1_003ED120", "0xE0") G1_ALIAS("D_003ED208", "g1_003ED120", "0xE8") G1_ALIAS("Pursuer_StateHewieHolds_ptmf", "g1_003ED120", "0xF0") G1_ALIAS("D_003ED218", "g1_003ED120", "0xF8") G1_ALIAS("Pursuer_StateBackOnFeet_ptmf", "g1_003ED120", "0x100") G1_ALIAS("D_003ED228", "g1_003ED120", "0x108") G1_ALIAS("Pursuer_StateBackOnFeet_ptmf2", "g1_003ED120", "0x110") G1_ALIAS("D_003ED238", "g1_003ED120", "0x118") G1_ALIAS("Pursuer_StateCountKeys_ptmf", "g1_003ED120", "0x120") G1_ALIAS("D_003ED248", "g1_003ED120", "0x128"));

/* 0x003ED410: Pursuer_StateHitReact_ptmf5, D_003ED418 */
void *g1_003ED410[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateHitReact, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateHitReact_ptmf5", "g1_003ED410", "0x0") G1_ALIAS("D_003ED418", "g1_003ED410", "0x8"));

/* 0x003EE710: Room00_Cmd00_ptmf, D_003EE718, Room00_Cmd01_ptmf, D_003EE724, D_003EE728, D_003EE730 ... */
void *g1_003EE710[12] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room00_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room00_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room00_HookNone, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room00_Cmd00_ptmf", "g1_003EE710", "0x0") G1_ALIAS("D_003EE718", "g1_003EE710", "0x8") G1_ALIAS("Room00_Cmd01_ptmf", "g1_003EE710", "0x10") G1_ALIAS("D_003EE724", "g1_003EE710", "0x14") G1_ALIAS("D_003EE728", "g1_003EE710", "0x18") G1_ALIAS("D_003EE730", "g1_003EE710", "0x20") G1_ALIAS("D_003EE738", "g1_003EE710", "0x28"));

/* 0x003F0378: Room02_Cmd00_ptmf, D_003F0380, Room02_Cmd01_ptmf, D_003F038C, D_003F0390, Room02_Cmd02_ptmf ... */
void *g1_003F0378[26] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room02_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room02_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room02_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room02_Cmd03, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room02_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room02_Cond01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room02_Cmd00_ptmf", "g1_003F0378", "0x0") G1_ALIAS("D_003F0380", "g1_003F0378", "0x8") G1_ALIAS("Room02_Cmd01_ptmf", "g1_003F0378", "0x10") G1_ALIAS("D_003F038C", "g1_003F0378", "0x14") G1_ALIAS("D_003F0390", "g1_003F0378", "0x18") G1_ALIAS("Room02_Cmd02_ptmf", "g1_003F0378", "0x20") G1_ALIAS("D_003F039C", "g1_003F0378", "0x24") G1_ALIAS("D_003F03A0", "g1_003F0378", "0x28") G1_ALIAS("Room02_Cmd03_ptmf", "g1_003F0378", "0x30") G1_ALIAS("D_003F03AC", "g1_003F0378", "0x34") G1_ALIAS("D_003F03B0", "g1_003F0378", "0x38") G1_ALIAS("Room02_Cond00_ptmf", "g1_003F0378", "0x40") G1_ALIAS("D_003F03C0", "g1_003F0378", "0x48") G1_ALIAS("Room02_Cond01_ptmf", "g1_003F0378", "0x50") G1_ALIAS("D_003F03CC", "g1_003F0378", "0x54") G1_ALIAS("D_003F03D0", "g1_003F0378", "0x58"));

/* 0x003F0D90: Room03_Cmd00_ptmf, D_003F0D98, Room03_Cmd01_ptmf, D_003F0DA4, D_003F0DA8 */
void *g1_003F0D90[8] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room03_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room03_Cmd01, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room03_Cmd00_ptmf", "g1_003F0D90", "0x0") G1_ALIAS("D_003F0D98", "g1_003F0D90", "0x8") G1_ALIAS("Room03_Cmd01_ptmf", "g1_003F0D90", "0x10") G1_ALIAS("D_003F0DA4", "g1_003F0D90", "0x14") G1_ALIAS("D_003F0DA8", "g1_003F0D90", "0x18"));

/* 0x003F1770: Room04_Cmd00_ptmf, D_003F1778, Room04_Cmd01_ptmf, D_003F1784, D_003F1788, Room04_Cmd02_ptmf ... */
void *g1_003F1770[16] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room04_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room04_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room04_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room04_Cmd03, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room04_Cmd00_ptmf", "g1_003F1770", "0x0") G1_ALIAS("D_003F1778", "g1_003F1770", "0x8") G1_ALIAS("Room04_Cmd01_ptmf", "g1_003F1770", "0x10") G1_ALIAS("D_003F1784", "g1_003F1770", "0x14") G1_ALIAS("D_003F1788", "g1_003F1770", "0x18") G1_ALIAS("Room04_Cmd02_ptmf", "g1_003F1770", "0x20") G1_ALIAS("D_003F1794", "g1_003F1770", "0x24") G1_ALIAS("D_003F1798", "g1_003F1770", "0x28") G1_ALIAS("Room04_Cmd03_ptmf", "g1_003F1770", "0x30") G1_ALIAS("D_003F17A4", "g1_003F1770", "0x34") G1_ALIAS("D_003F17A8", "g1_003F1770", "0x38"));

/* 0x003F21C0: Room06_Cmd00_ptmf, D_003F21C8 */
void *g1_003F21C0[4] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room06_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room06_Cmd00_ptmf", "g1_003F21C0", "0x0") G1_ALIAS("D_003F21C8", "g1_003F21C0", "0x8"));

/* 0x003F31C0: Room08_Cmd00_ptmf, D_003F31C8, Room08_Cmd01_ptmf, D_003F31D4, D_003F31D8, Room08_Cond00_ptmf ... */
void *g1_003F31C0[16] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room08_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room08_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room08_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room08_Cond01, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room08_Cmd00_ptmf", "g1_003F31C0", "0x0") G1_ALIAS("D_003F31C8", "g1_003F31C0", "0x8") G1_ALIAS("Room08_Cmd01_ptmf", "g1_003F31C0", "0x10") G1_ALIAS("D_003F31D4", "g1_003F31C0", "0x14") G1_ALIAS("D_003F31D8", "g1_003F31C0", "0x18") G1_ALIAS("Room08_Cond00_ptmf", "g1_003F31C0", "0x20") G1_ALIAS("D_003F31E8", "g1_003F31C0", "0x28") G1_ALIAS("Room08_Cond01_ptmf", "g1_003F31C0", "0x30") G1_ALIAS("D_003F31F4", "g1_003F31C0", "0x34") G1_ALIAS("D_003F31F8", "g1_003F31C0", "0x38"));

/* 0x003F3C50: Room09_Cond00_ptmf, D_003F3C58 */
void *g1_003F3C50[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room09_Cond00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room09_Cond00_ptmf", "g1_003F3C50", "0x0") G1_ALIAS("D_003F3C58", "g1_003F3C50", "0x8"));

/* 0x003F4378: Room0A_Cmd00_ptmf, D_003F4380, Room0A_Cmd01_ptmf, D_003F438C, D_003F4390 */
void *g1_003F4378[8] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room0A_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room0A_Cmd01, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room0A_Cmd00_ptmf", "g1_003F4378", "0x0") G1_ALIAS("D_003F4380", "g1_003F4378", "0x8") G1_ALIAS("Room0A_Cmd01_ptmf", "g1_003F4378", "0x10") G1_ALIAS("D_003F438C", "g1_003F4378", "0x14") G1_ALIAS("D_003F4390", "g1_003F4378", "0x18"));

/* 0x003F4640: Room0B_Cond00_ptmf, D_003F4648 */
void *g1_003F4640[4] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room0B_Cond00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room0B_Cond00_ptmf", "g1_003F4640", "0x0") G1_ALIAS("D_003F4648", "g1_003F4640", "0x8"));

/* 0x003F53F8: Room0C_Cmd00_ptmf, D_003F5400, Room0C_Cmd01_ptmf, D_003F540C, D_003F5410, Room0C_Cmd02_ptmf ... */
void *g1_003F53F8[18] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room0C_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room0C_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room0C_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room0C_Cmd03, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room0C_Cmd00_ptmf", "g1_003F53F8", "0x0") G1_ALIAS("D_003F5400", "g1_003F53F8", "0x8") G1_ALIAS("Room0C_Cmd01_ptmf", "g1_003F53F8", "0x10") G1_ALIAS("D_003F540C", "g1_003F53F8", "0x14") G1_ALIAS("D_003F5410", "g1_003F53F8", "0x18") G1_ALIAS("Room0C_Cmd02_ptmf", "g1_003F53F8", "0x20") G1_ALIAS("D_003F541C", "g1_003F53F8", "0x24") G1_ALIAS("D_003F5420", "g1_003F53F8", "0x28") G1_ALIAS("Room0C_Cmd03_ptmf", "g1_003F53F8", "0x30") G1_ALIAS("D_003F542C", "g1_003F53F8", "0x34") G1_ALIAS("D_003F5430", "g1_003F53F8", "0x38"));

/* 0x003F6EE8: Room0F_Cmd00_ptmf, D_003F6EF0, Room0F_Cmd01_ptmf, D_003F6EFC, D_003F6F00, Room0F_Cmd02_ptmf ... */
void *g1_003F6EE8[22] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room0F_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room0F_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room0F_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room0F_Cmd03, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room0F_Cmd04, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room0F_Cmd00_ptmf", "g1_003F6EE8", "0x0") G1_ALIAS("D_003F6EF0", "g1_003F6EE8", "0x8") G1_ALIAS("Room0F_Cmd01_ptmf", "g1_003F6EE8", "0x10") G1_ALIAS("D_003F6EFC", "g1_003F6EE8", "0x14") G1_ALIAS("D_003F6F00", "g1_003F6EE8", "0x18") G1_ALIAS("Room0F_Cmd02_ptmf", "g1_003F6EE8", "0x20") G1_ALIAS("D_003F6F0C", "g1_003F6EE8", "0x24") G1_ALIAS("D_003F6F10", "g1_003F6EE8", "0x28") G1_ALIAS("Room0F_Cmd03_ptmf", "g1_003F6EE8", "0x30") G1_ALIAS("D_003F6F1C", "g1_003F6EE8", "0x34") G1_ALIAS("D_003F6F20", "g1_003F6EE8", "0x38") G1_ALIAS("Room0F_Cmd04_ptmf", "g1_003F6EE8", "0x40") G1_ALIAS("D_003F6F2C", "g1_003F6EE8", "0x44") G1_ALIAS("D_003F6F30", "g1_003F6EE8", "0x48"));

/* 0x003F78F8: Room10_Cond00_ptmf, D_003F7900 */
void *g1_003F78F8[6] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room10_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room10_Cond00_ptmf", "g1_003F78F8", "0x0") G1_ALIAS("D_003F7900", "g1_003F78F8", "0x8"));

/* 0x003F9150: Room12_Cmd00_ptmf, D_003F9158, Room12_Cond00_ptmf, D_003F9168 */
void *g1_003F9150[8] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room12_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room12_Cond00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room12_Cmd00_ptmf", "g1_003F9150", "0x0") G1_ALIAS("D_003F9158", "g1_003F9150", "0x8") G1_ALIAS("Room12_Cond00_ptmf", "g1_003F9150", "0x10") G1_ALIAS("D_003F9168", "g1_003F9150", "0x18"));

/* 0x003F99A0: Room13_Cmd00_ptmf, D_003F99A8 */
void *g1_003F99A0[4] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room13_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room13_Cmd00_ptmf", "g1_003F99A0", "0x0") G1_ALIAS("D_003F99A8", "g1_003F99A0", "0x8"));

/* 0x003FA060: Room14_Cmd00_ptmf, D_003FA068 */
void *g1_003FA060[4] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room14_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room14_Cmd00_ptmf", "g1_003FA060", "0x0") G1_ALIAS("D_003FA068", "g1_003FA060", "0x8"));

/* 0x003FA728: Room15_Cmd00_ptmf, D_003FA730, Room15_Cond00_ptmf, D_003FA740 */
void *g1_003FA728[10] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room15_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room15_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room15_Cmd00_ptmf", "g1_003FA728", "0x0") G1_ALIAS("D_003FA730", "g1_003FA728", "0x8") G1_ALIAS("Room15_Cond00_ptmf", "g1_003FA728", "0x10") G1_ALIAS("D_003FA740", "g1_003FA728", "0x18"));

/* 0x003FB358: Room18_Cmd00_ptmf, D_003FB360 */
void *g1_003FB358[6] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room18_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room18_Cmd00_ptmf", "g1_003FB358", "0x0") G1_ALIAS("D_003FB360", "g1_003FB358", "0x8"));

/* 0x003FC010: Room19_Cond00_ptmf, D_003FC018 */
void *g1_003FC010[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room19_Cond00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room19_Cond00_ptmf", "g1_003FC010", "0x0") G1_ALIAS("D_003FC018", "g1_003FC010", "0x8"));

/* 0x003FC658: Room1A_Cmd00_ptmf, D_003FC660, Room1A_Cmd01_ptmf, D_003FC66C, D_003FC670 */
void *g1_003FC658[8] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room1A_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room1A_Cmd01, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room1A_Cmd00_ptmf", "g1_003FC658", "0x0") G1_ALIAS("D_003FC660", "g1_003FC658", "0x8") G1_ALIAS("Room1A_Cmd01_ptmf", "g1_003FC658", "0x10") G1_ALIAS("D_003FC66C", "g1_003FC658", "0x14") G1_ALIAS("D_003FC670", "g1_003FC658", "0x18"));

/* 0x003FCAF0: Room1B_Cmd00_ptmf, D_003FCAF8 */
void *g1_003FCAF0[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room1B_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room1B_Cmd00_ptmf", "g1_003FCAF0", "0x0") G1_ALIAS("D_003FCAF8", "g1_003FCAF0", "0x8"));

/* 0x003FD338: Room1C_Cmd00_ptmf, D_003FD340, Room1C_Cmd01_ptmf, D_003FD34C, D_003FD350 */
void *g1_003FD338[10] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room1C_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room1C_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room1C_Cmd00_ptmf", "g1_003FD338", "0x0") G1_ALIAS("D_003FD340", "g1_003FD338", "0x8") G1_ALIAS("Room1C_Cmd01_ptmf", "g1_003FD338", "0x10") G1_ALIAS("D_003FD34C", "g1_003FD338", "0x14") G1_ALIAS("D_003FD350", "g1_003FD338", "0x18"));

/* 0x003FD980: Room1D_Cmd00_ptmf, D_003FD988, Room1D_Cmd01_ptmf, D_003FD994, D_003FD998, Room1D_Cond00_ptmf ... */
void *g1_003FD980[12] __attribute__((aligned(128), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room1D_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room1D_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room1D_Cond00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room1D_Cmd00_ptmf", "g1_003FD980", "0x0") G1_ALIAS("D_003FD988", "g1_003FD980", "0x8") G1_ALIAS("Room1D_Cmd01_ptmf", "g1_003FD980", "0x10") G1_ALIAS("D_003FD994", "g1_003FD980", "0x14") G1_ALIAS("D_003FD998", "g1_003FD980", "0x18") G1_ALIAS("Room1D_Cond00_ptmf", "g1_003FD980", "0x20") G1_ALIAS("D_003FD9A8", "g1_003FD980", "0x28"));

/* 0x003FE440: Room1F_Cmd00_ptmf, D_003FE448 */
void *g1_003FE440[4] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room1F_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room1F_Cmd00_ptmf", "g1_003FE440", "0x0") G1_ALIAS("D_003FE448", "g1_003FE440", "0x8"));

/* 0x003FF078: Room20_Cmd00_ptmf, D_003FF080, Room20_Cmd01_ptmf, D_003FF08C, D_003FF090, Room20_Cmd02_ptmf ... */
void *g1_003FF078[38] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room20_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room20_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room20_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room20_Cmd03, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room20_Cmd04, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room20_Cmd05, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room20_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room20_Cond01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room20_Cond02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room20_Cmd00_ptmf", "g1_003FF078", "0x0") G1_ALIAS("D_003FF080", "g1_003FF078", "0x8") G1_ALIAS("Room20_Cmd01_ptmf", "g1_003FF078", "0x10") G1_ALIAS("D_003FF08C", "g1_003FF078", "0x14") G1_ALIAS("D_003FF090", "g1_003FF078", "0x18") G1_ALIAS("Room20_Cmd02_ptmf", "g1_003FF078", "0x20") G1_ALIAS("D_003FF09C", "g1_003FF078", "0x24") G1_ALIAS("D_003FF0A0", "g1_003FF078", "0x28") G1_ALIAS("Room20_Cmd03_ptmf", "g1_003FF078", "0x30") G1_ALIAS("D_003FF0AC", "g1_003FF078", "0x34") G1_ALIAS("D_003FF0B0", "g1_003FF078", "0x38") G1_ALIAS("Room20_Cmd04_ptmf", "g1_003FF078", "0x40") G1_ALIAS("D_003FF0BC", "g1_003FF078", "0x44") G1_ALIAS("D_003FF0C0", "g1_003FF078", "0x48") G1_ALIAS("Room20_Cmd05_ptmf", "g1_003FF078", "0x50") G1_ALIAS("D_003FF0CC", "g1_003FF078", "0x54") G1_ALIAS("D_003FF0D0", "g1_003FF078", "0x58") G1_ALIAS("Room20_Cond00_ptmf", "g1_003FF078", "0x60") G1_ALIAS("D_003FF0E0", "g1_003FF078", "0x68") G1_ALIAS("Room20_Cond01_ptmf", "g1_003FF078", "0x70") G1_ALIAS("D_003FF0EC", "g1_003FF078", "0x74") G1_ALIAS("D_003FF0F0", "g1_003FF078", "0x78") G1_ALIAS("Room20_Cond02_ptmf", "g1_003FF078", "0x80") G1_ALIAS("D_003FF0FC", "g1_003FF078", "0x84") G1_ALIAS("D_003FF100", "g1_003FF078", "0x88"));

/* 0x00400B80: Room21_Cmd00_ptmf, D_00400B88, Room21_Cmd01_ptmf, D_00400B94, D_00400B98, Room21_Cmd02_ptmf ... */
void *g1_00400B80[32] __attribute__((aligned(128), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room21_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room21_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room21_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room21_Cmd03, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room21_Cmd04, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room21_Cmd05, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room21_Cmd06, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room21_Cond00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room21_Cmd00_ptmf", "g1_00400B80", "0x0") G1_ALIAS("D_00400B88", "g1_00400B80", "0x8") G1_ALIAS("Room21_Cmd01_ptmf", "g1_00400B80", "0x10") G1_ALIAS("D_00400B94", "g1_00400B80", "0x14") G1_ALIAS("D_00400B98", "g1_00400B80", "0x18") G1_ALIAS("Room21_Cmd02_ptmf", "g1_00400B80", "0x20") G1_ALIAS("D_00400BA4", "g1_00400B80", "0x24") G1_ALIAS("D_00400BA8", "g1_00400B80", "0x28") G1_ALIAS("Room21_Cmd03_ptmf", "g1_00400B80", "0x30") G1_ALIAS("D_00400BB4", "g1_00400B80", "0x34") G1_ALIAS("D_00400BB8", "g1_00400B80", "0x38") G1_ALIAS("Room21_Cmd04_ptmf", "g1_00400B80", "0x40") G1_ALIAS("D_00400BC4", "g1_00400B80", "0x44") G1_ALIAS("D_00400BC8", "g1_00400B80", "0x48") G1_ALIAS("Room21_Cmd05_ptmf", "g1_00400B80", "0x50") G1_ALIAS("D_00400BD4", "g1_00400B80", "0x54") G1_ALIAS("D_00400BD8", "g1_00400B80", "0x58") G1_ALIAS("Room21_Cmd06_ptmf", "g1_00400B80", "0x60") G1_ALIAS("D_00400BE4", "g1_00400B80", "0x64") G1_ALIAS("D_00400BE8", "g1_00400B80", "0x68") G1_ALIAS("Room21_Cond00_ptmf", "g1_00400B80", "0x70") G1_ALIAS("D_00400BF8", "g1_00400B80", "0x78"));

/* 0x00401828: Room23_Cond00_ptmf, D_00401830, Room23_Cmd00_ptmf, D_00401840, Room23_Cmd01_ptmf, D_0040184C ... */
void *g1_00401828[14] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room23_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room23_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room23_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room23_Cond00_ptmf", "g1_00401828", "0x0") G1_ALIAS("D_00401830", "g1_00401828", "0x8") G1_ALIAS("Room23_Cmd00_ptmf", "g1_00401828", "0x10") G1_ALIAS("D_00401840", "g1_00401828", "0x18") G1_ALIAS("Room23_Cmd01_ptmf", "g1_00401828", "0x20") G1_ALIAS("D_0040184C", "g1_00401828", "0x24") G1_ALIAS("D_00401850", "g1_00401828", "0x28"));

/* 0x004022C0: Room24_KilnGlow_ptmf, D_004022C8, Room24_Smoke_ptmf, D_004022D4, D_004022D8, Room24_ColourPulse_ptmf ... */
void *g1_004022C0[12] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room24_KilnGlow, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room24_Smoke, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room24_ColourPulse, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room24_KilnGlow_ptmf", "g1_004022C0", "0x0") G1_ALIAS("D_004022C8", "g1_004022C0", "0x8") G1_ALIAS("Room24_Smoke_ptmf", "g1_004022C0", "0x10") G1_ALIAS("D_004022D4", "g1_004022C0", "0x14") G1_ALIAS("D_004022D8", "g1_004022C0", "0x18") G1_ALIAS("Room24_ColourPulse_ptmf", "g1_004022C0", "0x20") G1_ALIAS("D_004022E4", "g1_004022C0", "0x24") G1_ALIAS("D_004022E8", "g1_004022C0", "0x28"));

/* 0x00402D08: Room25_Cmd00_ptmf, D_00402D10, Room25_Cond00_ptmf, D_00402D20 */
void *g1_00402D08[10] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room25_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room25_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room25_Cmd00_ptmf", "g1_00402D08", "0x0") G1_ALIAS("D_00402D10", "g1_00402D08", "0x8") G1_ALIAS("Room25_Cond00_ptmf", "g1_00402D08", "0x10") G1_ALIAS("D_00402D20", "g1_00402D08", "0x18"));

/* 0x00403900: Room26_Cond00_ptmf, D_00403908, Room26_Cmd00_ptmf, D_00403918, Room26_Cmd01_ptmf, D_00403924 ... */
void *g1_00403900[16] __attribute__((aligned(128), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room26_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room26_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room26_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room26_Cmd02, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room26_Cond00_ptmf", "g1_00403900", "0x0") G1_ALIAS("D_00403908", "g1_00403900", "0x8") G1_ALIAS("Room26_Cmd00_ptmf", "g1_00403900", "0x10") G1_ALIAS("D_00403918", "g1_00403900", "0x18") G1_ALIAS("Room26_Cmd01_ptmf", "g1_00403900", "0x20") G1_ALIAS("D_00403924", "g1_00403900", "0x24") G1_ALIAS("D_00403928", "g1_00403900", "0x28") G1_ALIAS("Room26_Cmd02_ptmf", "g1_00403900", "0x30") G1_ALIAS("D_00403934", "g1_00403900", "0x34") G1_ALIAS("D_00403938", "g1_00403900", "0x38"));

/* 0x00403F68: Room28_Cond00_ptmf, D_00403F70 */
void *g1_00403F68[6] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room28_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room28_Cond00_ptmf", "g1_00403F68", "0x0") G1_ALIAS("D_00403F70", "g1_00403F68", "0x8"));

/* 0x004046E8: Room29_Cond00_ptmf, D_004046F0, Room29_Cond01_ptmf, D_004046FC, D_00404700 */
void *g1_004046E8[10] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room29_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room29_Cond01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room29_Cond00_ptmf", "g1_004046E8", "0x0") G1_ALIAS("D_004046F0", "g1_004046E8", "0x8") G1_ALIAS("Room29_Cond01_ptmf", "g1_004046E8", "0x10") G1_ALIAS("D_004046FC", "g1_004046E8", "0x14") G1_ALIAS("D_00404700", "g1_004046E8", "0x18"));

/* 0x004055D0: Room2A_Cmd00_ptmf, D_004055D8, Room2A_HandlerStep_ptmf, D_004055E4, D_004055E8, Room2A_Cmd02_ptmf ... */
void *g1_004055D0[16] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room2A_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room2A_HandlerStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room2A_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room2A_Cmd03, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room2A_Cmd00_ptmf", "g1_004055D0", "0x0") G1_ALIAS("D_004055D8", "g1_004055D0", "0x8") G1_ALIAS("Room2A_HandlerStep_ptmf", "g1_004055D0", "0x10") G1_ALIAS("D_004055E4", "g1_004055D0", "0x14") G1_ALIAS("D_004055E8", "g1_004055D0", "0x18") G1_ALIAS("Room2A_Cmd02_ptmf", "g1_004055D0", "0x20") G1_ALIAS("D_004055F4", "g1_004055D0", "0x24") G1_ALIAS("D_004055F8", "g1_004055D0", "0x28") G1_ALIAS("Room2A_Cmd03_ptmf", "g1_004055D0", "0x30") G1_ALIAS("D_00405604", "g1_004055D0", "0x34") G1_ALIAS("D_00405608", "g1_004055D0", "0x38"));

/* 0x00405A90: Room2B_Cmd00_ptmf, D_00405A98 */
void *g1_00405A90[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room2B_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room2B_Cmd00_ptmf", "g1_00405A90", "0x0") G1_ALIAS("D_00405A98", "g1_00405A90", "0x8"));

/* 0x004064C8: Room2D_Cmd00_ptmf, D_004064D0, Room2D_Cmd01_ptmf, D_004064DC, D_004064E0, Room2D_Cmd02_ptmf ... */
void *g1_004064C8[18] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room2D_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room2D_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room2D_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room2D_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room2D_Cmd00_ptmf", "g1_004064C8", "0x0") G1_ALIAS("D_004064D0", "g1_004064C8", "0x8") G1_ALIAS("Room2D_Cmd01_ptmf", "g1_004064C8", "0x10") G1_ALIAS("D_004064DC", "g1_004064C8", "0x14") G1_ALIAS("D_004064E0", "g1_004064C8", "0x18") G1_ALIAS("Room2D_Cmd02_ptmf", "g1_004064C8", "0x20") G1_ALIAS("D_004064EC", "g1_004064C8", "0x24") G1_ALIAS("D_004064F0", "g1_004064C8", "0x28") G1_ALIAS("Room2D_Cond00_ptmf", "g1_004064C8", "0x30") G1_ALIAS("D_00406500", "g1_004064C8", "0x38"));

/* 0x00407078: Room40_Cmd00_ptmf, D_00407080 */
void *g1_00407078[6] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room40_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room40_Cmd00_ptmf", "g1_00407078", "0x0") G1_ALIAS("D_00407080", "g1_00407078", "0x8"));

/* 0x00407E38: Room43_Cmd00_ptmf, D_00407E40 */
void *g1_00407E38[6] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room43_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room43_Cmd00_ptmf", "g1_00407E38", "0x0") G1_ALIAS("D_00407E40", "g1_00407E38", "0x8"));

/* 0x00407F18: Room45_Cmd00_ptmf, D_00407F20 */
void *g1_00407F18[6] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room45_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room45_Cmd00_ptmf", "g1_00407F18", "0x0") G1_ALIAS("D_00407F20", "g1_00407F18", "0x8"));

/* 0x00408638: Room49_Cmd00_ptmf, D_00408640, Room49_Cmd01_ptmf, D_0040864C, D_00408650, Room49_Cmd02_ptmf ... */
void *g1_00408638[14] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room49_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room49_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room49_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room49_Cmd00_ptmf", "g1_00408638", "0x0") G1_ALIAS("D_00408640", "g1_00408638", "0x8") G1_ALIAS("Room49_Cmd01_ptmf", "g1_00408638", "0x10") G1_ALIAS("D_0040864C", "g1_00408638", "0x14") G1_ALIAS("D_00408650", "g1_00408638", "0x18") G1_ALIAS("Room49_Cmd02_ptmf", "g1_00408638", "0x20") G1_ALIAS("D_0040865C", "g1_00408638", "0x24") G1_ALIAS("D_00408660", "g1_00408638", "0x28"));

/* 0x00409908: Room4B_Cmd00_ptmf, D_00409910, Room4B_Cmd01_ptmf, D_0040991C, D_00409920, Room4B_Cond00_ptmf ... */
void *g1_00409908[12] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room4B_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room4B_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room4B_Cond00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room4B_Cmd00_ptmf", "g1_00409908", "0x0") G1_ALIAS("D_00409910", "g1_00409908", "0x8") G1_ALIAS("Room4B_Cmd01_ptmf", "g1_00409908", "0x10") G1_ALIAS("D_0040991C", "g1_00409908", "0x14") G1_ALIAS("D_00409920", "g1_00409908", "0x18") G1_ALIAS("Room4B_Cond00_ptmf", "g1_00409908", "0x20") G1_ALIAS("D_00409930", "g1_00409908", "0x28"));

/* 0x0040ABE0: Room4C_Cmd00_ptmf, D_0040ABE8, Room4C_Cmd01_ptmf, D_0040ABF4, D_0040ABF8, Room4C_Cmd02_ptmf ... */
void *g1_0040ABE0[12] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room4C_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room4C_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room4C_Cmd02, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room4C_Cmd00_ptmf", "g1_0040ABE0", "0x0") G1_ALIAS("D_0040ABE8", "g1_0040ABE0", "0x8") G1_ALIAS("Room4C_Cmd01_ptmf", "g1_0040ABE0", "0x10") G1_ALIAS("D_0040ABF4", "g1_0040ABE0", "0x14") G1_ALIAS("D_0040ABF8", "g1_0040ABE0", "0x18") G1_ALIAS("Room4C_Cmd02_ptmf", "g1_0040ABE0", "0x20") G1_ALIAS("D_0040AC04", "g1_0040ABE0", "0x24") G1_ALIAS("D_0040AC08", "g1_0040ABE0", "0x28"));

/* 0x0040B4D0: Room4E_Cmd00_ptmf, D_0040B4D8, Room4E_Cmd01_ptmf, D_0040B4E4, D_0040B4E8, Room4E_Cmd02_ptmf ... */
void *g1_0040B4D0[12] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room4E_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room4E_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room4E_Cmd02, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room4E_Cmd00_ptmf", "g1_0040B4D0", "0x0") G1_ALIAS("D_0040B4D8", "g1_0040B4D0", "0x8") G1_ALIAS("Room4E_Cmd01_ptmf", "g1_0040B4D0", "0x10") G1_ALIAS("D_0040B4E4", "g1_0040B4D0", "0x14") G1_ALIAS("D_0040B4E8", "g1_0040B4D0", "0x18") G1_ALIAS("Room4E_Cmd02_ptmf", "g1_0040B4D0", "0x20") G1_ALIAS("D_0040B4F4", "g1_0040B4D0", "0x24") G1_ALIAS("D_0040B4F8", "g1_0040B4D0", "0x28"));

/* 0x0040C0E8: Room4F_Cmd00_ptmf, D_0040C0F0, Room4F_Cmd01_ptmf, D_0040C0FC, D_0040C100, Room4F_Cmd02_ptmf ... */
void *g1_0040C0E8[22] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room4F_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room4F_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room4F_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room4F_Cmd03, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room4F_CreatureVanish, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room4F_Cmd00_ptmf", "g1_0040C0E8", "0x0") G1_ALIAS("D_0040C0F0", "g1_0040C0E8", "0x8") G1_ALIAS("Room4F_Cmd01_ptmf", "g1_0040C0E8", "0x10") G1_ALIAS("D_0040C0FC", "g1_0040C0E8", "0x14") G1_ALIAS("D_0040C100", "g1_0040C0E8", "0x18") G1_ALIAS("Room4F_Cmd02_ptmf", "g1_0040C0E8", "0x20") G1_ALIAS("D_0040C10C", "g1_0040C0E8", "0x24") G1_ALIAS("D_0040C110", "g1_0040C0E8", "0x28") G1_ALIAS("Room4F_Cmd03_ptmf", "g1_0040C0E8", "0x30") G1_ALIAS("D_0040C11C", "g1_0040C0E8", "0x34") G1_ALIAS("D_0040C120", "g1_0040C0E8", "0x38") G1_ALIAS("Room4F_CreatureVanish_ptmf", "g1_0040C0E8", "0x40") G1_ALIAS("D_0040C130", "g1_0040C0E8", "0x48"));

/* 0x0040CF00: Room50_Cmd00_ptmf, D_0040CF08 */
void *g1_0040CF00[4] __attribute__((aligned(128), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room50_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room50_Cmd00_ptmf", "g1_0040CF00", "0x0") G1_ALIAS("D_0040CF08", "g1_0040CF00", "0x8"));

/* 0x0040E340: Room51_Cmd00_ptmf, D_0040E348, Room51_Cmd01_ptmf, D_0040E354, D_0040E358 */
void *g1_0040E340[8] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room51_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room51_Cmd01, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room51_Cmd00_ptmf", "g1_0040E340", "0x0") G1_ALIAS("D_0040E348", "g1_0040E340", "0x8") G1_ALIAS("Room51_Cmd01_ptmf", "g1_0040E340", "0x10") G1_ALIAS("D_0040E354", "g1_0040E340", "0x14") G1_ALIAS("D_0040E358", "g1_0040E340", "0x18"));

/* 0x0040ECC8: Room52_Cmd00_ptmf, D_0040ECD0, Room52_Cond00_ptmf, D_0040ECE0 */
void *g1_0040ECC8[10] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room52_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room52_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room52_Cmd00_ptmf", "g1_0040ECC8", "0x0") G1_ALIAS("D_0040ECD0", "g1_0040ECC8", "0x8") G1_ALIAS("Room52_Cond00_ptmf", "g1_0040ECC8", "0x10") G1_ALIAS("D_0040ECE0", "g1_0040ECC8", "0x18"));

/* 0x0040F468: Room56_Cmd00_ptmf, D_0040F470 */
void *g1_0040F468[4] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room56_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room56_Cmd00_ptmf", "g1_0040F468", "0x0") G1_ALIAS("D_0040F470", "g1_0040F468", "0x8"));

/* 0x00410B80: Room59_Cmd00_ptmf, D_00410B88 */
void *g1_00410B80[4] __attribute__((aligned(128), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room59_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room59_Cmd00_ptmf", "g1_00410B80", "0x0") G1_ALIAS("D_00410B88", "g1_00410B80", "0x8"));

/* 0x00410F28: Room5A_Cmd00_ptmf, D_00410F30 */
void *g1_00410F28[4] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room5A_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room5A_Cmd00_ptmf", "g1_00410F28", "0x0") G1_ALIAS("D_00410F30", "g1_00410F28", "0x8"));

/* 0x00411B40: Room5C_Cmd00_ptmf, D_00411B48, Room5C_Cond00_ptmf, D_00411B58 */
void *g1_00411B40[8] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room5C_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room5C_Cond00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room5C_Cmd00_ptmf", "g1_00411B40", "0x0") G1_ALIAS("D_00411B48", "g1_00411B40", "0x8") G1_ALIAS("Room5C_Cond00_ptmf", "g1_00411B40", "0x10") G1_ALIAS("D_00411B58", "g1_00411B40", "0x18"));

/* 0x004123B0: Room5D_Cmd00_ptmf, D_004123B8, Room5D_Cmd01_ptmf, D_004123C4, D_004123C8 */
void *g1_004123B0[8] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room5D_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room5D_Cmd01, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room5D_Cmd00_ptmf", "g1_004123B0", "0x0") G1_ALIAS("D_004123B8", "g1_004123B0", "0x8") G1_ALIAS("Room5D_Cmd01_ptmf", "g1_004123B0", "0x10") G1_ALIAS("D_004123C4", "g1_004123B0", "0x14") G1_ALIAS("D_004123C8", "g1_004123B0", "0x18"));

/* 0x004126A8: D_004126A8, D_004126B0, D_004126B8, D_004126C0 */
void *g1_004126A8[8] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Movie_StateStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Movie_StateOpening, (void *)0x00000000u,
};
__asm__(G1_ALIAS("D_004126A8", "g1_004126A8", "0x0") G1_ALIAS("D_004126B0", "g1_004126A8", "0x8") G1_ALIAS("D_004126B8", "g1_004126A8", "0x10") G1_ALIAS("D_004126C0", "g1_004126A8", "0x18"));

/* 0x004126D8: Movie_StateWaitFirst_ptmf, D_004126E0, Movie_StatePausedFirst_ptmf, D_004126F0 */
void *g1_004126D8[8] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Movie_StateWaitFirst, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Movie_StatePausedFirst, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Movie_StateWaitFirst_ptmf", "g1_004126D8", "0x0") G1_ALIAS("D_004126E0", "g1_004126D8", "0x8") G1_ALIAS("Movie_StatePausedFirst_ptmf", "g1_004126D8", "0x10") G1_ALIAS("D_004126F0", "g1_004126D8", "0x18"));

/* 0x00412730: Movie_StateStart_ptmf, D_00412738, Movie_StateStart_ptmf2, D_00412748, Movie_StateStart_ptmf3, D_00412758 ... */
void *g1_00412730[16] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Movie_StateStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Movie_StateStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Movie_StateStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateIdle, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Movie_StateStart_ptmf", "g1_00412730", "0x0") G1_ALIAS("D_00412738", "g1_00412730", "0x8") G1_ALIAS("Movie_StateStart_ptmf2", "g1_00412730", "0x10") G1_ALIAS("D_00412748", "g1_00412730", "0x18") G1_ALIAS("Movie_StateStart_ptmf3", "g1_00412730", "0x20") G1_ALIAS("D_00412758", "g1_00412730", "0x28") G1_ALIAS("D_00412760", "g1_00412730", "0x30") G1_ALIAS("D_00412768", "g1_00412730", "0x38"));

/* 0x00412890: MusicDir_GlobalFade_ptmf, D_00412894, D_00412898, MusicDir_TrackFade_ptmf, D_004128A4, D_004128A8 ... */
void *g1_00412890[48] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)MusicDir_GlobalFade, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)MusicDir_TrackFade, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)MusicDir_SeqFade, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)MusicDir_BendFade, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Movie_StateStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Movie_StateStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Movie_StateStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Cutscene_Start, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Cutscene_ClearStatus, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Cutscene_StateScript, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Cutscene_StateFirstShot, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Cutscene_StatePlaying, (void *)0x00000000u,
};
__asm__(G1_ALIAS("MusicDir_GlobalFade_ptmf", "g1_00412890", "0x0") G1_ALIAS("D_00412894", "g1_00412890", "0x4") G1_ALIAS("D_00412898", "g1_00412890", "0x8") G1_ALIAS("MusicDir_TrackFade_ptmf", "g1_00412890", "0x10") G1_ALIAS("D_004128A4", "g1_00412890", "0x14") G1_ALIAS("D_004128A8", "g1_00412890", "0x18") G1_ALIAS("MusicDir_SeqFade_ptmf", "g1_00412890", "0x20") G1_ALIAS("D_004128B4", "g1_00412890", "0x24") G1_ALIAS("D_004128B8", "g1_00412890", "0x28") G1_ALIAS("MusicDir_BendFade_ptmf", "g1_00412890", "0x30") G1_ALIAS("D_004128C4", "g1_00412890", "0x34") G1_ALIAS("D_004128C8", "g1_00412890", "0x38") G1_ALIAS("Movie_StateStart_ptmf4", "g1_00412890", "0x40") G1_ALIAS("D_004128D8", "g1_00412890", "0x48") G1_ALIAS("Movie_StateStart_ptmf5", "g1_00412890", "0x50") G1_ALIAS("D_004128E8", "g1_00412890", "0x58") G1_ALIAS("D_004128F0", "g1_00412890", "0x60") G1_ALIAS("D_004128F8", "g1_00412890", "0x68") G1_ALIAS("Cutscene_Start_ptmf", "g1_00412890", "0x70") G1_ALIAS("D_00412904", "g1_00412890", "0x74") G1_ALIAS("D_00412908", "g1_00412890", "0x78") G1_ALIAS("Cutscene_ClearStatus_ptmf", "g1_00412890", "0x80") G1_ALIAS("D_00412914", "g1_00412890", "0x84") G1_ALIAS("D_00412918", "g1_00412890", "0x88") G1_ALIAS("Cutscene_StateScript_ptmf", "g1_00412890", "0x90") G1_ALIAS("D_00412924", "g1_00412890", "0x94") G1_ALIAS("D_00412928", "g1_00412890", "0x98") G1_ALIAS("Cutscene_StateFirstShot_ptmf", "g1_00412890", "0xA0") G1_ALIAS("D_00412934", "g1_00412890", "0xA4") G1_ALIAS("D_00412938", "g1_00412890", "0xA8") G1_ALIAS("Cutscene_StatePlaying_ptmf", "g1_00412890", "0xB0") G1_ALIAS("D_00412944", "g1_00412890", "0xB4") G1_ALIAS("D_00412948", "g1_00412890", "0xB8"));

/* 0x00412E80: Room2F_Cond00_ptmf, D_00412E88 */
void *g1_00412E80[4] __attribute__((aligned(128), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room2F_Cond00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room2F_Cond00_ptmf", "g1_00412E80", "0x0") G1_ALIAS("D_00412E88", "g1_00412E80", "0x8"));

/* 0x004134B0: Room30_Cond00_ptmf, D_004134B8 */
void *g1_004134B0[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room30_Cond00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room30_Cond00_ptmf", "g1_004134B0", "0x0") G1_ALIAS("D_004134B8", "g1_004134B0", "0x8"));

/* 0x00413590: Debilitas3_StateNone_ptmf, D_00413598, Debilitas3_StartTurnToFiona_ptmf, D_004135A4, D_004135A8, Debilitas3_StartLunge_ptmf ... */
void *g1_00413590[16] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas3_StateNone, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas3_StartTurnToFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas3_StartLunge, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas3_StartGrab, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Debilitas3_StateNone_ptmf", "g1_00413590", "0x0") G1_ALIAS("D_00413598", "g1_00413590", "0x8") G1_ALIAS("Debilitas3_StartTurnToFiona_ptmf", "g1_00413590", "0x10") G1_ALIAS("D_004135A4", "g1_00413590", "0x14") G1_ALIAS("D_004135A8", "g1_00413590", "0x18") G1_ALIAS("Debilitas3_StartLunge_ptmf", "g1_00413590", "0x20") G1_ALIAS("D_004135B4", "g1_00413590", "0x24") G1_ALIAS("D_004135B8", "g1_00413590", "0x28") G1_ALIAS("Debilitas3_StartGrab_ptmf", "g1_00413590", "0x30") G1_ALIAS("D_004135C4", "g1_00413590", "0x34") G1_ALIAS("D_004135C8", "g1_00413590", "0x38"));

/* 0x00414290: Debilitas3_StateTurnToFiona_ptmf, D_00414298, Debilitas3_StateBlow_ptmf, D_004142A8, Debilitas3_StateStun_ptmf, D_004142B8 ... */
void *g1_00414290[28] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas3_StateTurnToFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas3_StateBlow, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas3_StateStun, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas3_StateStun, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas3_StateLunge, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_AttackNextStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas3_StateGrab, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Debilitas3_StateTurnToFiona_ptmf", "g1_00414290", "0x0") G1_ALIAS("D_00414298", "g1_00414290", "0x8") G1_ALIAS("Debilitas3_StateBlow_ptmf", "g1_00414290", "0x10") G1_ALIAS("D_004142A8", "g1_00414290", "0x18") G1_ALIAS("Debilitas3_StateStun_ptmf", "g1_00414290", "0x20") G1_ALIAS("D_004142B8", "g1_00414290", "0x28") G1_ALIAS("Debilitas3_StateStun_ptmf2", "g1_00414290", "0x30") G1_ALIAS("D_004142C8", "g1_00414290", "0x38") G1_ALIAS("Debilitas3_StateLunge_ptmf", "g1_00414290", "0x40") G1_ALIAS("D_004142D8", "g1_00414290", "0x48") G1_ALIAS("Pursuer_AttackNextStep_ptmf5", "g1_00414290", "0x50") G1_ALIAS("D_004142E8", "g1_00414290", "0x58") G1_ALIAS("Debilitas3_StateGrab_ptmf", "g1_00414290", "0x60") G1_ALIAS("D_004142F8", "g1_00414290", "0x68"));

/* 0x00414340: sTaskIdleState, D_00414348 */
void *g1_00414340[4] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateIdle, (void *)0x00000000u,
};
__asm__(G1_ALIAS("sTaskIdleState", "g1_00414340", "0x0") G1_ALIAS("D_00414348", "g1_00414340", "0x8"));

/* 0x00414370: sGameStateMain, D_00414374, D_00414378, sGameStateShutdown, D_00414384, D_00414388 */
void *g1_00414370[8] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Game_StateMain, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Game_StateShutdown, (void *)0x00000000u,
};
__asm__(G1_ALIAS("sGameStateMain", "g1_00414370", "0x0") G1_ALIAS("D_00414374", "g1_00414370", "0x4") G1_ALIAS("D_00414378", "g1_00414370", "0x8") G1_ALIAS("sGameStateShutdown", "g1_00414370", "0x10") G1_ALIAS("D_00414384", "g1_00414370", "0x14") G1_ALIAS("D_00414388", "g1_00414370", "0x18"));

/* 0x004147A0: Ball_StateRolling_ptmf, D_004147A8, Ball_StateDropping_ptmf, D_004147B8 */
void *g1_004147A0[8] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Ball_StateRolling, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Ball_StateDropping, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Ball_StateRolling_ptmf", "g1_004147A0", "0x0") G1_ALIAS("D_004147A8", "g1_004147A0", "0x8") G1_ALIAS("Ball_StateDropping_ptmf", "g1_004147A0", "0x10") G1_ALIAS("D_004147B8", "g1_004147A0", "0x18"));

/* 0x00414880: Riccardo_StartFlurry_ptmf, D_00414888, Riccardo_StartLunge_ptmf, D_00414894, D_00414898 */
void *g1_00414880[8] __attribute__((aligned(128), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Riccardo_StartFlurry, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Riccardo_StartLunge, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Riccardo_StartFlurry_ptmf", "g1_00414880", "0x0") G1_ALIAS("D_00414888", "g1_00414880", "0x8") G1_ALIAS("Riccardo_StartLunge_ptmf", "g1_00414880", "0x10") G1_ALIAS("D_00414894", "g1_00414880", "0x14") G1_ALIAS("D_00414898", "g1_00414880", "0x18"));

/* 0x00415670: Riccardo_StateBehaviour_ptmf, D_00415678 */
void *g1_00415670[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Riccardo_StateBehaviour, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Riccardo_StateBehaviour_ptmf", "g1_00415670", "0x0") G1_ALIAS("D_00415678", "g1_00415670", "0x8"));

/* 0x00415690: Riccardo_StateChase_ptmf, D_00415698 */
void *g1_00415690[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Riccardo_StateChase, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Riccardo_StateChase_ptmf", "g1_00415690", "0x0") G1_ALIAS("D_00415698", "g1_00415690", "0x8"));

/* 0x004156F8: Riccardo_FlurryBlow_ptmf, D_00415700, Riccardo_StateBlow_ptmf, D_00415710, Riccardo_StateBlowHewie_ptmf, D_00415720 ... */
void *g1_004156F8[42] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Riccardo_FlurryBlow, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Riccardo_StateBlow, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Riccardo_StateBlowHewie, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Riccardo_StateBlowFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Riccardo_StateAfterBlow, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Riccardo_StateAfterBlow, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Riccardo_StateAfterBlow, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Riccardo_FlurryBlow, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Riccardo_StateLunge, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_AttackNextStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Riccardo_FlurryBlow_ptmf", "g1_004156F8", "0x0") G1_ALIAS("D_00415700", "g1_004156F8", "0x8") G1_ALIAS("Riccardo_StateBlow_ptmf", "g1_004156F8", "0x10") G1_ALIAS("D_00415710", "g1_004156F8", "0x18") G1_ALIAS("Riccardo_StateBlowHewie_ptmf", "g1_004156F8", "0x20") G1_ALIAS("D_00415720", "g1_004156F8", "0x28") G1_ALIAS("Riccardo_StateBlowFiona_ptmf", "g1_004156F8", "0x30") G1_ALIAS("D_00415730", "g1_004156F8", "0x38") G1_ALIAS("Riccardo_StateAfterBlow_ptmf", "g1_004156F8", "0x40") G1_ALIAS("D_00415740", "g1_004156F8", "0x48") G1_ALIAS("Riccardo_StateAfterBlow_ptmf2", "g1_004156F8", "0x50") G1_ALIAS("D_00415750", "g1_004156F8", "0x58") G1_ALIAS("Riccardo_StateAfterBlow_ptmf3", "g1_004156F8", "0x60") G1_ALIAS("D_00415760", "g1_004156F8", "0x68") G1_ALIAS("Riccardo_FlurryBlow_ptmf2", "g1_004156F8", "0x70") G1_ALIAS("D_00415770", "g1_004156F8", "0x78") G1_ALIAS("Riccardo_StateLunge_ptmf", "g1_004156F8", "0x80") G1_ALIAS("D_00415780", "g1_004156F8", "0x88") G1_ALIAS("Pursuer_AttackNextStep_ptmf6", "g1_004156F8", "0x90") G1_ALIAS("D_00415790", "g1_004156F8", "0x98"));

/* 0x004166B0: CreatureA_StateVanish_ptmf, D_004166B8, CreatureA_StateVanish_ptmf2, D_004166C8, CreatureA_StateVanish_ptmf3, D_004166D8 ... */
void *g1_004166B0[60] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureA_StateVanish, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureA_StateVanish, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureA_StateVanish, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureA_StateNone, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureA_StateNone, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureA_StateHangAbout, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureA_StateNone, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureA_StateAfterFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureA_StateNone, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureA_StateNone, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureA_StateToDoor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureA_StateNone, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureA_StateRoute, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureA_StateNone, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureA_StateVanish, (void *)0x00000000u,
};
__asm__(G1_ALIAS("CreatureA_StateVanish_ptmf", "g1_004166B0", "0x0") G1_ALIAS("D_004166B8", "g1_004166B0", "0x8") G1_ALIAS("CreatureA_StateVanish_ptmf2", "g1_004166B0", "0x10") G1_ALIAS("D_004166C8", "g1_004166B0", "0x18") G1_ALIAS("CreatureA_StateVanish_ptmf3", "g1_004166B0", "0x20") G1_ALIAS("D_004166D8", "g1_004166B0", "0x28") G1_ALIAS("CreatureA_StateNone_ptmf", "g1_004166B0", "0x30") G1_ALIAS("D_004166E8", "g1_004166B0", "0x38") G1_ALIAS("CreatureA_StateNone_ptmf2", "g1_004166B0", "0x40") G1_ALIAS("D_004166F8", "g1_004166B0", "0x48") G1_ALIAS("CreatureA_StateHangAbout_ptmf", "g1_004166B0", "0x50") G1_ALIAS("D_00416708", "g1_004166B0", "0x58") G1_ALIAS("CreatureA_StateNone_ptmf3", "g1_004166B0", "0x60") G1_ALIAS("D_00416718", "g1_004166B0", "0x68") G1_ALIAS("CreatureA_StateAfterFiona_ptmf", "g1_004166B0", "0x70") G1_ALIAS("D_00416728", "g1_004166B0", "0x78") G1_ALIAS("CreatureA_StateNone_ptmf4", "g1_004166B0", "0x80") G1_ALIAS("D_00416738", "g1_004166B0", "0x88") G1_ALIAS("CreatureA_StateNone_ptmf5", "g1_004166B0", "0x90") G1_ALIAS("D_00416748", "g1_004166B0", "0x98") G1_ALIAS("CreatureA_StateToDoor_ptmf", "g1_004166B0", "0xA0") G1_ALIAS("D_00416758", "g1_004166B0", "0xA8") G1_ALIAS("CreatureA_StateNone_ptmf6", "g1_004166B0", "0xB0") G1_ALIAS("D_00416768", "g1_004166B0", "0xB8") G1_ALIAS("CreatureA_StateRoute_ptmf", "g1_004166B0", "0xC0") G1_ALIAS("D_00416778", "g1_004166B0", "0xC8") G1_ALIAS("CreatureA_StateNone_ptmf7", "g1_004166B0", "0xD0") G1_ALIAS("D_00416788", "g1_004166B0", "0xD8") G1_ALIAS("CreatureA_StateVanish_ptmf4", "g1_004166B0", "0xE0") G1_ALIAS("D_00416798", "g1_004166B0", "0xE8"));

/* 0x00417148: Room46_CharHook_ptmf, D_00417150, Room46_PursuerBusy_ptmf, D_00417160 */
void *g1_00417148[10] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room46_CharHook, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room46_PursuerBusy, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room46_CharHook_ptmf", "g1_00417148", "0x0") G1_ALIAS("D_00417150", "g1_00417148", "0x8") G1_ALIAS("Room46_PursuerBusy_ptmf", "g1_00417148", "0x10") G1_ALIAS("D_00417160", "g1_00417148", "0x18"));

/* 0x00417810: Room102_Cmd00_ptmf, D_00417818 */
void *g1_00417810[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room102_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room102_Cmd00_ptmf", "g1_00417810", "0x0") G1_ALIAS("D_00417818", "g1_00417810", "0x8"));

/* 0x00417BA0: Room103_Cmd00_ptmf, D_00417BA8 */
void *g1_00417BA0[4] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room103_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room103_Cmd00_ptmf", "g1_00417BA0", "0x0") G1_ALIAS("D_00417BA8", "g1_00417BA0", "0x8"));

/* 0x00417F20: Room104_Cmd00_ptmf, D_00417F28 */
void *g1_00417F20[4] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room104_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room104_Cmd00_ptmf", "g1_00417F20", "0x0") G1_ALIAS("D_00417F28", "g1_00417F20", "0x8"));

/* 0x004182A0: Room105_Cmd00_ptmf, D_004182A8 */
void *g1_004182A0[4] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room105_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room105_Cmd00_ptmf", "g1_004182A0", "0x0") G1_ALIAS("D_004182A8", "g1_004182A0", "0x8"));

/* 0x00418AB8: Room106_Cmd00_ptmf, D_00418AC0 */
void *g1_00418AB8[6] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room106_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room106_Cmd00_ptmf", "g1_00418AB8", "0x0") G1_ALIAS("D_00418AC0", "g1_00418AB8", "0x8"));

/* 0x00418D80: Room107_Cond00_ptmf, D_00418D88, Room107_Cmd00_ptmf, D_00418D98, Room107_Cmd01_ptmf, D_00418DA4 ... */
void *g1_00418D80[12] __attribute__((aligned(128), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room107_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room107_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room107_Cmd01, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room107_Cond00_ptmf", "g1_00418D80", "0x0") G1_ALIAS("D_00418D88", "g1_00418D80", "0x8") G1_ALIAS("Room107_Cmd00_ptmf", "g1_00418D80", "0x10") G1_ALIAS("D_00418D98", "g1_00418D80", "0x18") G1_ALIAS("Room107_Cmd01_ptmf", "g1_00418D80", "0x20") G1_ALIAS("D_00418DA4", "g1_00418D80", "0x24") G1_ALIAS("D_00418DA8", "g1_00418D80", "0x28"));

/* 0x00419388: Room108_Cmd00_ptmf, D_00419390 */
void *g1_00419388[6] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room108_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room108_Cmd00_ptmf", "g1_00419388", "0x0") G1_ALIAS("D_00419390", "g1_00419388", "0x8"));

/* 0x004196F0: Room109_Cmd00_ptmf, D_004196F8, Room109_Cmd01_ptmf, D_00419704, D_00419708 */
void *g1_004196F0[8] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room109_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room109_Cmd01, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room109_Cmd00_ptmf", "g1_004196F0", "0x0") G1_ALIAS("D_004196F8", "g1_004196F0", "0x8") G1_ALIAS("Room109_Cmd01_ptmf", "g1_004196F0", "0x10") G1_ALIAS("D_00419704", "g1_004196F0", "0x14") G1_ALIAS("D_00419708", "g1_004196F0", "0x18"));

/* 0x00419A40: Room10A_Cond00_ptmf, D_00419A48, Room10A_Cmd00_ptmf, D_00419A58 */
void *g1_00419A40[8] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room10A_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room10A_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room10A_Cond00_ptmf", "g1_00419A40", "0x0") G1_ALIAS("D_00419A48", "g1_00419A40", "0x8") G1_ALIAS("Room10A_Cmd00_ptmf", "g1_00419A40", "0x10") G1_ALIAS("D_00419A58", "g1_00419A40", "0x18"));

/* 0x00419D88: Room10B_Cmd00_ptmf, D_00419D90, Room10B_Cond00_ptmf, D_00419DA0 */
void *g1_00419D88[8] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room10B_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room10B_Cond00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room10B_Cmd00_ptmf", "g1_00419D88", "0x0") G1_ALIAS("D_00419D90", "g1_00419D88", "0x8") G1_ALIAS("Room10B_Cond00_ptmf", "g1_00419D88", "0x10") G1_ALIAS("D_00419DA0", "g1_00419D88", "0x18"));

/* 0x00419E50: Pursuer_StateRunThenNext_ptmf6, D_00419E58 */
void *g1_00419E50[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf6", "g1_00419E50", "0x0") G1_ALIAS("D_00419E58", "g1_00419E50", "0x8"));

/* 0x0041A150: GameOver_StateStart_ptmf, D_0041A154, D_0041A158, GameOver_StateStart_ptmf2, D_0041A164, D_0041A168 ... */
void *g1_0041A150[92] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)GameOver_StateStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)GameOver_StateStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)GameOver_StateMovie, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)GameOver_StateOthers, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)GameOver_StateDone, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)GameOver_StateMode3, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)GameOver_StateOthers, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)GameOver_StateSpecial, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pause_StateOpening, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)MoviePause_StateOpening, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pause_StateOpen, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pause_StateClosing, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pause_StateMenu, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pause_StateClosing, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pause_StateMenuClosing, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pause_StateQuitAsk, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pause_StateQuit, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pause_StateOpen, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pause_StateOpen, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)MoviePause_StateOpen, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)MoviePause_StateSkip, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)MoviePause_StateClosing, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)MoviePause_StateClosing, (void *)0x00000000u,
};
__asm__(G1_ALIAS("GameOver_StateStart_ptmf", "g1_0041A150", "0x0") G1_ALIAS("D_0041A154", "g1_0041A150", "0x4") G1_ALIAS("D_0041A158", "g1_0041A150", "0x8") G1_ALIAS("GameOver_StateStart_ptmf2", "g1_0041A150", "0x10") G1_ALIAS("D_0041A164", "g1_0041A150", "0x14") G1_ALIAS("D_0041A168", "g1_0041A150", "0x18") G1_ALIAS("GameOver_StateMovie_ptmf", "g1_0041A150", "0x20") G1_ALIAS("D_0041A174", "g1_0041A150", "0x24") G1_ALIAS("D_0041A178", "g1_0041A150", "0x28") G1_ALIAS("GameOver_StateOthers_ptmf", "g1_0041A150", "0x30") G1_ALIAS("D_0041A184", "g1_0041A150", "0x34") G1_ALIAS("D_0041A188", "g1_0041A150", "0x38") G1_ALIAS("GameOver_StateDone_ptmf", "g1_0041A150", "0x40") G1_ALIAS("D_0041A194", "g1_0041A150", "0x44") G1_ALIAS("D_0041A198", "g1_0041A150", "0x48") G1_ALIAS("GameOver_StateMode3_ptmf", "g1_0041A150", "0x50") G1_ALIAS("D_0041A1A4", "g1_0041A150", "0x54") G1_ALIAS("D_0041A1A8", "g1_0041A150", "0x58") G1_ALIAS("GameOver_StateOthers_ptmf2", "g1_0041A150", "0x60") G1_ALIAS("D_0041A1B4", "g1_0041A150", "0x64") G1_ALIAS("D_0041A1B8", "g1_0041A150", "0x68") G1_ALIAS("GameOver_StateSpecial_ptmf", "g1_0041A150", "0x70") G1_ALIAS("D_0041A1C4", "g1_0041A150", "0x74") G1_ALIAS("D_0041A1C8", "g1_0041A150", "0x78") G1_ALIAS("Pause_StateOpening_ptmf", "g1_0041A150", "0x80") G1_ALIAS("D_0041A1D4", "g1_0041A150", "0x84") G1_ALIAS("D_0041A1D8", "g1_0041A150", "0x88") G1_ALIAS("MoviePause_StateOpening_ptmf", "g1_0041A150", "0x90") G1_ALIAS("D_0041A1E4", "g1_0041A150", "0x94") G1_ALIAS("D_0041A1E8", "g1_0041A150", "0x98") G1_ALIAS("Pause_StateOpen_ptmf", "g1_0041A150", "0xA0") G1_ALIAS("D_0041A1F4", "g1_0041A150", "0xA4") G1_ALIAS("D_0041A1F8", "g1_0041A150", "0xA8") G1_ALIAS("Pause_StateClosing_ptmf", "g1_0041A150", "0xB0") G1_ALIAS("D_0041A204", "g1_0041A150", "0xB4") G1_ALIAS("D_0041A208", "g1_0041A150", "0xB8") G1_ALIAS("Pause_StateMenu_ptmf", "g1_0041A150", "0xC0") G1_ALIAS("D_0041A214", "g1_0041A150", "0xC4") G1_ALIAS("D_0041A218", "g1_0041A150", "0xC8") G1_ALIAS("Pause_StateClosing_ptmf2", "g1_0041A150", "0xD0") G1_ALIAS("D_0041A224", "g1_0041A150", "0xD4") G1_ALIAS("D_0041A228", "g1_0041A150", "0xD8") G1_ALIAS("Pause_StateMenuClosing_ptmf", "g1_0041A150", "0xE0") G1_ALIAS("D_0041A234", "g1_0041A150", "0xE4") G1_ALIAS("D_0041A238", "g1_0041A150", "0xE8") G1_ALIAS("Pause_StateQuitAsk_ptmf", "g1_0041A150", "0xF0") G1_ALIAS("D_0041A244", "g1_0041A150", "0xF4") G1_ALIAS("D_0041A248", "g1_0041A150", "0xF8") G1_ALIAS("Pause_StateQuit_ptmf", "g1_0041A150", "0x100") G1_ALIAS("D_0041A254", "g1_0041A150", "0x104") G1_ALIAS("D_0041A258", "g1_0041A150", "0x108") G1_ALIAS("Pause_StateOpen_ptmf2", "g1_0041A150", "0x110") G1_ALIAS("D_0041A264", "g1_0041A150", "0x114") G1_ALIAS("D_0041A268", "g1_0041A150", "0x118") G1_ALIAS("Pause_StateOpen_ptmf3", "g1_0041A150", "0x120") G1_ALIAS("D_0041A274", "g1_0041A150", "0x124") G1_ALIAS("D_0041A278", "g1_0041A150", "0x128") G1_ALIAS("MoviePause_StateOpen_ptmf", "g1_0041A150", "0x130") G1_ALIAS("D_0041A284", "g1_0041A150", "0x134") G1_ALIAS("D_0041A288", "g1_0041A150", "0x138") G1_ALIAS("MoviePause_StateSkip_ptmf", "g1_0041A150", "0x140") G1_ALIAS("D_0041A294", "g1_0041A150", "0x144") G1_ALIAS("D_0041A298", "g1_0041A150", "0x148") G1_ALIAS("MoviePause_StateClosing_ptmf", "g1_0041A150", "0x150") G1_ALIAS("D_0041A2A4", "g1_0041A150", "0x154") G1_ALIAS("D_0041A2A8", "g1_0041A150", "0x158") G1_ALIAS("MoviePause_StateClosing_ptmf2", "g1_0041A150", "0x160") G1_ALIAS("D_0041A2B4", "g1_0041A150", "0x164") G1_ALIAS("D_0041A2B8", "g1_0041A150", "0x168"));

/* 0x0041B630: Room_EmptyHook_ptmf, D_0041B638, D_0041B640, D_0041B644, D_0041B648, D_0041B650 ... */
void *g1_0041B630[16] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room_EmptyHook, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas2_StartTurnToFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas2_StartLunge, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas2_StartGrab, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room_EmptyHook_ptmf", "g1_0041B630", "0x0") G1_ALIAS("D_0041B638", "g1_0041B630", "0x8") G1_ALIAS("D_0041B640", "g1_0041B630", "0x10") G1_ALIAS("D_0041B644", "g1_0041B630", "0x14") G1_ALIAS("D_0041B648", "g1_0041B630", "0x18") G1_ALIAS("D_0041B650", "g1_0041B630", "0x20") G1_ALIAS("D_0041B654", "g1_0041B630", "0x24") G1_ALIAS("D_0041B658", "g1_0041B630", "0x28") G1_ALIAS("D_0041B660", "g1_0041B630", "0x30") G1_ALIAS("D_0041B664", "g1_0041B630", "0x34") G1_ALIAS("D_0041B668", "g1_0041B630", "0x38"));

/* 0x0041C310: D_0041C310, D_0041C318, D_0041C320, D_0041C328, D_0041C330, D_0041C338 ... */
void *g1_0041C310[28] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas2_StateTurnToFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas2_StateBlow, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas2_StateStun, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas2_StateStun, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas2_StateLunge, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_AttackNextStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Debilitas2_StateGrab, (void *)0x00000000u,
};
__asm__(G1_ALIAS("D_0041C310", "g1_0041C310", "0x0") G1_ALIAS("D_0041C318", "g1_0041C310", "0x8") G1_ALIAS("D_0041C320", "g1_0041C310", "0x10") G1_ALIAS("D_0041C328", "g1_0041C310", "0x18") G1_ALIAS("D_0041C330", "g1_0041C310", "0x20") G1_ALIAS("D_0041C338", "g1_0041C310", "0x28") G1_ALIAS("D_0041C340", "g1_0041C310", "0x30") G1_ALIAS("D_0041C348", "g1_0041C310", "0x38") G1_ALIAS("D_0041C350", "g1_0041C310", "0x40") G1_ALIAS("D_0041C358", "g1_0041C310", "0x48") G1_ALIAS("Pursuer_AttackNextStep_ptmf7", "g1_0041C310", "0x50") G1_ALIAS("D_0041C368", "g1_0041C310", "0x58") G1_ALIAS("D_0041C370", "g1_0041C310", "0x60") G1_ALIAS("D_0041C378", "g1_0041C310", "0x68"));

/* 0x0041CA70: Room27_Cmd00_ptmf, D_0041CA78, Movie_StateStart_ptmf6, D_0041CA88 */
void *g1_0041CA70[8] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room27_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Movie_StateStart, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room27_Cmd00_ptmf", "g1_0041CA70", "0x0") G1_ALIAS("D_0041CA78", "g1_0041CA70", "0x8") G1_ALIAS("Movie_StateStart_ptmf6", "g1_0041CA70", "0x10") G1_ALIAS("D_0041CA88", "g1_0041CA70", "0x18"));

/* 0x0041D7B8: Room2E_Cmd00_ptmf, D_0041D7C0, Room2E_Cmd01_ptmf, D_0041D7CC, D_0041D7D0, Room2E_Cmd02_ptmf ... */
void *g1_0041D7B8[20] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room2E_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room2E_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room2E_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room2E_Cmd03, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room2E_Cmd04, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room2E_Cmd00_ptmf", "g1_0041D7B8", "0x0") G1_ALIAS("D_0041D7C0", "g1_0041D7B8", "0x8") G1_ALIAS("Room2E_Cmd01_ptmf", "g1_0041D7B8", "0x10") G1_ALIAS("D_0041D7CC", "g1_0041D7B8", "0x14") G1_ALIAS("D_0041D7D0", "g1_0041D7B8", "0x18") G1_ALIAS("Room2E_Cmd02_ptmf", "g1_0041D7B8", "0x20") G1_ALIAS("D_0041D7DC", "g1_0041D7B8", "0x24") G1_ALIAS("D_0041D7E0", "g1_0041D7B8", "0x28") G1_ALIAS("Room2E_Cmd03_ptmf", "g1_0041D7B8", "0x30") G1_ALIAS("D_0041D7EC", "g1_0041D7B8", "0x34") G1_ALIAS("D_0041D7F0", "g1_0041D7B8", "0x38") G1_ALIAS("Room2E_Cmd04_ptmf", "g1_0041D7B8", "0x40") G1_ALIAS("D_0041D7FC", "g1_0041D7B8", "0x44") G1_ALIAS("D_0041D800", "g1_0041D7B8", "0x48"));

/* 0x0041F550: Room66_CharHook_ptmf, D_0041F558, Room66_Effect_ptmf, D_0041F564, D_0041F568, Room66_Sink_ptmf ... */
void *g1_0041F550[28] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room66_CharHook, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room66_Effect, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room66_Sink, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room66_Fire, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room66_ColourPulse, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room66_Letters, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room66_CharPast, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room66_CharHook_ptmf", "g1_0041F550", "0x0") G1_ALIAS("D_0041F558", "g1_0041F550", "0x8") G1_ALIAS("Room66_Effect_ptmf", "g1_0041F550", "0x10") G1_ALIAS("D_0041F564", "g1_0041F550", "0x14") G1_ALIAS("D_0041F568", "g1_0041F550", "0x18") G1_ALIAS("Room66_Sink_ptmf", "g1_0041F550", "0x20") G1_ALIAS("D_0041F574", "g1_0041F550", "0x24") G1_ALIAS("D_0041F578", "g1_0041F550", "0x28") G1_ALIAS("Room66_Fire_ptmf", "g1_0041F550", "0x30") G1_ALIAS("D_0041F584", "g1_0041F550", "0x34") G1_ALIAS("D_0041F588", "g1_0041F550", "0x38") G1_ALIAS("Room66_ColourPulse_ptmf", "g1_0041F550", "0x40") G1_ALIAS("D_0041F594", "g1_0041F550", "0x44") G1_ALIAS("D_0041F598", "g1_0041F550", "0x48") G1_ALIAS("Room66_Letters_ptmf", "g1_0041F550", "0x50") G1_ALIAS("D_0041F5A8", "g1_0041F550", "0x58") G1_ALIAS("Room66_CharPast_ptmf", "g1_0041F550", "0x60") G1_ALIAS("D_0041F5B4", "g1_0041F550", "0x64") G1_ALIAS("D_0041F5B8", "g1_0041F550", "0x68"));

/* 0x004210F0: Room55_Effect_ptmf, D_004210F8 */
void *g1_004210F0[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room55_Effect, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room55_Effect_ptmf", "g1_004210F0", "0x0") G1_ALIAS("D_004210F8", "g1_004210F0", "0x8"));

/* 0x00421BE0: Room63_Cmd00_ptmf, D_00421BE8 */
void *g1_00421BE0[4] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room63_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room63_Cmd00_ptmf", "g1_00421BE0", "0x0") G1_ALIAS("D_00421BE8", "g1_00421BE0", "0x8"));

/* 0x00422300: Room62_Cmd00_ptmf, D_00422308, Room62_Cmd01_ptmf, D_00422314, D_00422318 */
void *g1_00422300[8] __attribute__((aligned(128), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room62_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room62_Cmd01, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room62_Cmd00_ptmf", "g1_00422300", "0x0") G1_ALIAS("D_00422308", "g1_00422300", "0x8") G1_ALIAS("Room62_Cmd01_ptmf", "g1_00422300", "0x10") G1_ALIAS("D_00422314", "g1_00422300", "0x14") G1_ALIAS("D_00422318", "g1_00422300", "0x18"));

/* 0x00422420: Pursuer_StateRunThenNext_ptmf7, D_00422428 */
void *g1_00422420[4] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf7", "g1_00422420", "0x0") G1_ALIAS("D_00422428", "g1_00422420", "0x8"));

/* 0x00422660: Lorenzo2_StartSink_ptmf, D_00422668, Lorenzo2_StartStalkBelow_ptmf, D_00422674, D_00422678, Lorenzo2_StartGrab_ptmf ... */
void *g1_00422660[12] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Lorenzo2_StartSink, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Lorenzo2_StartStalkBelow, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Lorenzo2_StartGrab, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Lorenzo2_StartSink_ptmf", "g1_00422660", "0x0") G1_ALIAS("D_00422668", "g1_00422660", "0x8") G1_ALIAS("Lorenzo2_StartStalkBelow_ptmf", "g1_00422660", "0x10") G1_ALIAS("D_00422674", "g1_00422660", "0x14") G1_ALIAS("D_00422678", "g1_00422660", "0x18") G1_ALIAS("Lorenzo2_StartGrab_ptmf", "g1_00422660", "0x20") G1_ALIAS("D_00422684", "g1_00422660", "0x24") G1_ALIAS("D_00422688", "g1_00422660", "0x28"));

/* 0x00423A28: Lorenzo2_StateSinkBehind_ptmf, D_00423A30, Lorenzo2_StateSink_ptmf, D_00423A40, Lorenzo2_StateUnderFloor_ptmf, D_00423A50 ... */
void *g1_00423A28[38] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Lorenzo2_StateSinkBehind, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Lorenzo2_StateSink, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Lorenzo2_StateUnderFloor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Lorenzo2_StatePlayOut, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Lorenzo2_StateApproach, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Lorenzo2_StateAB80, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Lorenzo2_StateRise, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Lorenzo2_StateSweep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Lorenzo2_StateGrab, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Lorenzo2_StateSinkBehind_ptmf", "g1_00423A28", "0x0") G1_ALIAS("D_00423A30", "g1_00423A28", "0x8") G1_ALIAS("Lorenzo2_StateSink_ptmf", "g1_00423A28", "0x10") G1_ALIAS("D_00423A40", "g1_00423A28", "0x18") G1_ALIAS("Lorenzo2_StateUnderFloor_ptmf", "g1_00423A28", "0x20") G1_ALIAS("D_00423A50", "g1_00423A28", "0x28") G1_ALIAS("Lorenzo2_StatePlayOut_ptmf", "g1_00423A28", "0x30") G1_ALIAS("D_00423A60", "g1_00423A28", "0x38") G1_ALIAS("Lorenzo2_StateApproach_ptmf", "g1_00423A28", "0x40") G1_ALIAS("D_00423A70", "g1_00423A28", "0x48") G1_ALIAS("Lorenzo2_StateAB80_ptmf", "g1_00423A28", "0x50") G1_ALIAS("D_00423A80", "g1_00423A28", "0x58") G1_ALIAS("Lorenzo2_StateRise_ptmf", "g1_00423A28", "0x60") G1_ALIAS("D_00423A90", "g1_00423A28", "0x68") G1_ALIAS("Lorenzo2_StateSweep_ptmf", "g1_00423A28", "0x70") G1_ALIAS("D_00423AA0", "g1_00423A28", "0x78") G1_ALIAS("Lorenzo2_StateGrab_ptmf", "g1_00423A28", "0x80") G1_ALIAS("D_00423AB0", "g1_00423A28", "0x88"));

/* 0x004267F0: Room48_Cmd00_ptmf, D_004267F8, Room48_Cmd01_ptmf, D_00426804, D_00426808, Room48_Cmd02_ptmf ... */
void *g1_004267F0[28] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room48_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room48_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room48_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room48_Cmd03, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room48_Cmd04, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room48_Cmd05, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room48_Cond00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room48_Cmd00_ptmf", "g1_004267F0", "0x0") G1_ALIAS("D_004267F8", "g1_004267F0", "0x8") G1_ALIAS("Room48_Cmd01_ptmf", "g1_004267F0", "0x10") G1_ALIAS("D_00426804", "g1_004267F0", "0x14") G1_ALIAS("D_00426808", "g1_004267F0", "0x18") G1_ALIAS("Room48_Cmd02_ptmf", "g1_004267F0", "0x20") G1_ALIAS("D_00426814", "g1_004267F0", "0x24") G1_ALIAS("D_00426818", "g1_004267F0", "0x28") G1_ALIAS("Room48_Cmd03_ptmf", "g1_004267F0", "0x30") G1_ALIAS("D_00426824", "g1_004267F0", "0x34") G1_ALIAS("D_00426828", "g1_004267F0", "0x38") G1_ALIAS("Room48_Cmd04_ptmf", "g1_004267F0", "0x40") G1_ALIAS("D_00426834", "g1_004267F0", "0x44") G1_ALIAS("D_00426838", "g1_004267F0", "0x48") G1_ALIAS("Room48_Cmd05_ptmf", "g1_004267F0", "0x50") G1_ALIAS("D_00426844", "g1_004267F0", "0x54") G1_ALIAS("D_00426848", "g1_004267F0", "0x58") G1_ALIAS("Room48_Cond00_ptmf", "g1_004267F0", "0x60") G1_ALIAS("D_00426858", "g1_004267F0", "0x68"));

/* 0x00428028: Room54_GroupStep_ptmf, D_00428030, Room54_DoorRegions_ptmf, D_0042803C, D_00428040 */
void *g1_00428028[10] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room54_GroupStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room54_DoorRegions, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room54_GroupStep_ptmf", "g1_00428028", "0x0") G1_ALIAS("D_00428030", "g1_00428028", "0x8") G1_ALIAS("Room54_DoorRegions_ptmf", "g1_00428028", "0x10") G1_ALIAS("D_0042803C", "g1_00428028", "0x14") G1_ALIAS("D_00428040", "g1_00428028", "0x18"));

/* 0x00429140: Room60_Cmd00_ptmf, D_00429148, Room60_Cmd01_ptmf, D_00429154, D_00429158, Room60_Cmd02_ptmf ... */
void *g1_00429140[24] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room60_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room60_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room60_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room60_Cmd03, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room60_Cmd04, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room60_Cmd05, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room60_Cmd00_ptmf", "g1_00429140", "0x0") G1_ALIAS("D_00429148", "g1_00429140", "0x8") G1_ALIAS("Room60_Cmd01_ptmf", "g1_00429140", "0x10") G1_ALIAS("D_00429154", "g1_00429140", "0x14") G1_ALIAS("D_00429158", "g1_00429140", "0x18") G1_ALIAS("Room60_Cmd02_ptmf", "g1_00429140", "0x20") G1_ALIAS("D_00429164", "g1_00429140", "0x24") G1_ALIAS("D_00429168", "g1_00429140", "0x28") G1_ALIAS("Room60_Cmd03_ptmf", "g1_00429140", "0x30") G1_ALIAS("D_00429174", "g1_00429140", "0x34") G1_ALIAS("D_00429178", "g1_00429140", "0x38") G1_ALIAS("Room60_Cmd04_ptmf", "g1_00429140", "0x40") G1_ALIAS("D_00429184", "g1_00429140", "0x44") G1_ALIAS("D_00429188", "g1_00429140", "0x48") G1_ALIAS("Room60_Cmd05_ptmf", "g1_00429140", "0x50") G1_ALIAS("D_00429194", "g1_00429140", "0x54") G1_ALIAS("D_00429198", "g1_00429140", "0x58"));

/* 0x00429558: Room61_Cmd00_ptmf, D_00429560, Room61_Cmd01_ptmf, D_0042956C, D_00429570 */
void *g1_00429558[10] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room61_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room61_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room61_Cmd00_ptmf", "g1_00429558", "0x0") G1_ALIAS("D_00429560", "g1_00429558", "0x8") G1_ALIAS("Room61_Cmd01_ptmf", "g1_00429558", "0x10") G1_ALIAS("D_0042956C", "g1_00429558", "0x14") G1_ALIAS("D_00429570", "g1_00429558", "0x18"));

/* 0x004297B0: Pursuer_StateRunThenNext_ptmf8, D_004297B8 */
void *g1_004297B0[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf8", "g1_004297B0", "0x0") G1_ALIAS("D_004297B8", "g1_004297B0", "0x8"));

/* 0x00429840: Pursuer_StateRunThenNext_ptmf9, D_00429848 */
void *g1_00429840[4] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf9", "g1_00429840", "0x0") G1_ALIAS("D_00429848", "g1_00429840", "0x8"));

/* 0x00429C38: Thing01_StateDropping_ptmf, D_00429C40 */
void *g1_00429C38[6] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Thing01_StateDropping, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Thing01_StateDropping_ptmf", "g1_00429C38", "0x0") G1_ALIAS("D_00429C40", "g1_00429C38", "0x8"));

/* 0x00429CD0: Pursuer_StateRunThenNext_ptmf10, D_00429CD8 */
void *g1_00429CD0[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf10", "g1_00429CD0", "0x0") G1_ALIAS("D_00429CD8", "g1_00429CD0", "0x8"));

/* 0x00429D60: Pursuer_StateRunThenNext_ptmf11, D_00429D68 */
void *g1_00429D60[4] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf11", "g1_00429D60", "0x0") G1_ALIAS("D_00429D68", "g1_00429D60", "0x8"));

/* 0x00429DF0: Pursuer_StateRunThenNext_ptmf12, D_00429DF8 */
void *g1_00429DF0[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf12", "g1_00429DF0", "0x0") G1_ALIAS("D_00429DF8", "g1_00429DF0", "0x8"));

/* 0x0042A0B0: Room31_RoomEffect_ptmf, D_0042A0B8, Room31_Fan_ptmf, D_0042A0C4, D_0042A0C8, Room31_WindowLight_ptmf ... */
void *g1_0042A0B0[12] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room31_RoomEffect, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room31_Fan, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room31_WindowLight, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room31_RoomEffect_ptmf", "g1_0042A0B0", "0x0") G1_ALIAS("D_0042A0B8", "g1_0042A0B0", "0x8") G1_ALIAS("Room31_Fan_ptmf", "g1_0042A0B0", "0x10") G1_ALIAS("D_0042A0C4", "g1_0042A0B0", "0x14") G1_ALIAS("D_0042A0C8", "g1_0042A0B0", "0x18") G1_ALIAS("Room31_WindowLight_ptmf", "g1_0042A0B0", "0x20") G1_ALIAS("D_0042A0D4", "g1_0042A0B0", "0x24") G1_ALIAS("D_0042A0D8", "g1_0042A0B0", "0x28"));

/* 0x0042A170: Pursuer_StateRunThenNext_ptmf13, D_0042A178 */
void *g1_0042A170[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf13", "g1_0042A170", "0x0") G1_ALIAS("D_0042A178", "g1_0042A170", "0x8"));

/* 0x0042C290: Room32_Cmd00_ptmf, D_0042C298, Room32_Cmd01_ptmf, D_0042C2A4, D_0042C2A8, Room32_Cmd02_ptmf ... */
void *g1_0042C290[24] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room32_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room32_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room32_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room32_Cmd03, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room32_Cmd04, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room32_Cmd05, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room32_Cmd00_ptmf", "g1_0042C290", "0x0") G1_ALIAS("D_0042C298", "g1_0042C290", "0x8") G1_ALIAS("Room32_Cmd01_ptmf", "g1_0042C290", "0x10") G1_ALIAS("D_0042C2A4", "g1_0042C290", "0x14") G1_ALIAS("D_0042C2A8", "g1_0042C290", "0x18") G1_ALIAS("Room32_Cmd02_ptmf", "g1_0042C290", "0x20") G1_ALIAS("D_0042C2B4", "g1_0042C290", "0x24") G1_ALIAS("D_0042C2B8", "g1_0042C290", "0x28") G1_ALIAS("Room32_Cmd03_ptmf", "g1_0042C290", "0x30") G1_ALIAS("D_0042C2C4", "g1_0042C290", "0x34") G1_ALIAS("D_0042C2C8", "g1_0042C290", "0x38") G1_ALIAS("Room32_Cmd04_ptmf", "g1_0042C290", "0x40") G1_ALIAS("D_0042C2D4", "g1_0042C290", "0x44") G1_ALIAS("D_0042C2D8", "g1_0042C290", "0x48") G1_ALIAS("Room32_Cmd05_ptmf", "g1_0042C290", "0x50") G1_ALIAS("D_0042C2E4", "g1_0042C290", "0x54") G1_ALIAS("D_0042C2E8", "g1_0042C290", "0x58"));

/* 0x0042C400: Pursuer_StateRunThenNext_ptmf14, D_0042C408, SynthPot_StateDebug_ptmf, D_0042C418, SlotMachine_StateChoose_ptmf, D_0042C428 ... */
void *g1_0042C400[24] __attribute__((aligned(128), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SynthPot_StateDebug, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SlotMachine_StateChoose, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SlotMachine_StateNothing, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SlotMachine_StateRoll, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)ItemFound_StateShow, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf14", "g1_0042C400", "0x0") G1_ALIAS("D_0042C408", "g1_0042C400", "0x8") G1_ALIAS("SynthPot_StateDebug_ptmf", "g1_0042C400", "0x10") G1_ALIAS("D_0042C418", "g1_0042C400", "0x18") G1_ALIAS("SlotMachine_StateChoose_ptmf", "g1_0042C400", "0x20") G1_ALIAS("D_0042C428", "g1_0042C400", "0x28") G1_ALIAS("SlotMachine_StateNothing_ptmf", "g1_0042C400", "0x30") G1_ALIAS("D_0042C438", "g1_0042C400", "0x38") G1_ALIAS("SlotMachine_StateRoll_ptmf", "g1_0042C400", "0x40") G1_ALIAS("D_0042C448", "g1_0042C400", "0x48") G1_ALIAS("ItemFound_StateShow_ptmf", "g1_0042C400", "0x50") G1_ALIAS("D_0042C458", "g1_0042C400", "0x58"));

/* 0x0042C4B0: CreatureB_StateWatch0_ptmf, D_0042C4B8, CreatureB_StateWatch1_ptmf, D_0042C4C8, CreatureB_StateHit_ptmf, D_0042C4D8 ... */
void *g1_0042C4B0[88] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateWatch0, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateWatch1, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateHit, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateGrab, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateLeaving, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateAfterFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateToDoor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateApproach10, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateFlag9, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateFionaOffMesh, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateKnockedDown, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateFionaCaught, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateTurnToFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateWalkSpot, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateByProgress, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateFionaInSight, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateDoor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateHeldOff, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateNone, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateRoute, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateLeaving, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)CreatureB_StateNone, (void *)0x00000000u,
};
__asm__(G1_ALIAS("CreatureB_StateWatch0_ptmf", "g1_0042C4B0", "0x0") G1_ALIAS("D_0042C4B8", "g1_0042C4B0", "0x8") G1_ALIAS("CreatureB_StateWatch1_ptmf", "g1_0042C4B0", "0x10") G1_ALIAS("D_0042C4C8", "g1_0042C4B0", "0x18") G1_ALIAS("CreatureB_StateHit_ptmf", "g1_0042C4B0", "0x20") G1_ALIAS("D_0042C4D8", "g1_0042C4B0", "0x28") G1_ALIAS("CreatureB_StateGrab_ptmf", "g1_0042C4B0", "0x30") G1_ALIAS("D_0042C4E8", "g1_0042C4B0", "0x38") G1_ALIAS("CreatureB_StateLeaving_ptmf", "g1_0042C4B0", "0x40") G1_ALIAS("D_0042C4F8", "g1_0042C4B0", "0x48") G1_ALIAS("CreatureB_StateAfterFiona_ptmf", "g1_0042C4B0", "0x50") G1_ALIAS("D_0042C508", "g1_0042C4B0", "0x58") G1_ALIAS("CreatureB_StateToDoor_ptmf", "g1_0042C4B0", "0x60") G1_ALIAS("D_0042C518", "g1_0042C4B0", "0x68") G1_ALIAS("CreatureB_StateApproach10_ptmf", "g1_0042C4B0", "0x70") G1_ALIAS("D_0042C528", "g1_0042C4B0", "0x78") G1_ALIAS("CreatureB_StateFlag9_ptmf", "g1_0042C4B0", "0x80") G1_ALIAS("D_0042C538", "g1_0042C4B0", "0x88") G1_ALIAS("CreatureB_StateFionaOffMesh_ptmf", "g1_0042C4B0", "0x90") G1_ALIAS("D_0042C548", "g1_0042C4B0", "0x98") G1_ALIAS("CreatureB_StateKnockedDown_ptmf", "g1_0042C4B0", "0xA0") G1_ALIAS("D_0042C558", "g1_0042C4B0", "0xA8") G1_ALIAS("CreatureB_StateFionaCaught_ptmf", "g1_0042C4B0", "0xB0") G1_ALIAS("D_0042C568", "g1_0042C4B0", "0xB8") G1_ALIAS("CreatureB_StateTurnToFiona_ptmf", "g1_0042C4B0", "0xC0") G1_ALIAS("D_0042C578", "g1_0042C4B0", "0xC8") G1_ALIAS("CreatureB_StateWalkSpot_ptmf", "g1_0042C4B0", "0xD0") G1_ALIAS("D_0042C588", "g1_0042C4B0", "0xD8") G1_ALIAS("CreatureB_StateByProgress_ptmf", "g1_0042C4B0", "0xE0") G1_ALIAS("D_0042C598", "g1_0042C4B0", "0xE8") G1_ALIAS("CreatureB_StateFionaInSight_ptmf", "g1_0042C4B0", "0xF0") G1_ALIAS("D_0042C5A8", "g1_0042C4B0", "0xF8") G1_ALIAS("CreatureB_StateDoor_ptmf", "g1_0042C4B0", "0x100") G1_ALIAS("D_0042C5B8", "g1_0042C4B0", "0x108") G1_ALIAS("CreatureB_StateHeldOff_ptmf", "g1_0042C4B0", "0x110") G1_ALIAS("D_0042C5C8", "g1_0042C4B0", "0x118") G1_ALIAS("CreatureB_StateNone_ptmf", "g1_0042C4B0", "0x120") G1_ALIAS("D_0042C5D8", "g1_0042C4B0", "0x128") G1_ALIAS("CreatureB_StateRoute_ptmf", "g1_0042C4B0", "0x130") G1_ALIAS("D_0042C5E8", "g1_0042C4B0", "0x138") G1_ALIAS("CreatureB_StateLeaving_ptmf2", "g1_0042C4B0", "0x140") G1_ALIAS("D_0042C5F8", "g1_0042C4B0", "0x148") G1_ALIAS("CreatureB_StateNone_ptmf2", "g1_0042C4B0", "0x150") G1_ALIAS("D_0042C608", "g1_0042C4B0", "0x158"));

/* 0x0042C720: Pursuer_StateRunThenNext_ptmf15, D_0042C728 */
void *g1_0042C720[4] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf15", "g1_0042C720", "0x0") G1_ALIAS("D_0042C728", "g1_0042C720", "0x8"));

/* 0x0042C850: Room86_Cmd00_ptmf, D_0042C858 */
void *g1_0042C850[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room86_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room86_Cmd00_ptmf", "g1_0042C850", "0x0") G1_ALIAS("D_0042C858", "g1_0042C850", "0x8"));

/* 0x0042C8F0: Pursuer_StateRunThenNext_ptmf16, D_0042C8F8 */
void *g1_0042C8F0[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf16", "g1_0042C8F0", "0x0") G1_ALIAS("D_0042C8F8", "g1_0042C8F0", "0x8"));

/* 0x0042C980: Pursuer_StateRunThenNext_ptmf17, D_0042C988 */
void *g1_0042C980[4] __attribute__((aligned(128), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf17", "g1_0042C980", "0x0") G1_ALIAS("D_0042C988", "g1_0042C980", "0x8"));

/* 0x0042CA10: Pursuer_StateRunThenNext_ptmf18, D_0042CA18 */
void *g1_0042CA10[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf18", "g1_0042CA10", "0x0") G1_ALIAS("D_0042CA18", "g1_0042CA10", "0x8"));

/* 0x0042E390: RoomC0_Cmd00_ptmf, D_0042E398, RoomC0_Cmd01_ptmf, D_0042E3A4, D_0042E3A8, RoomC0_Cmd02_ptmf ... */
void *g1_0042E390[24] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC0_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC0_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC0_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC0_Cmd03, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC0_Cmd04, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC0_Cond00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomC0_Cmd00_ptmf", "g1_0042E390", "0x0") G1_ALIAS("D_0042E398", "g1_0042E390", "0x8") G1_ALIAS("RoomC0_Cmd01_ptmf", "g1_0042E390", "0x10") G1_ALIAS("D_0042E3A4", "g1_0042E390", "0x14") G1_ALIAS("D_0042E3A8", "g1_0042E390", "0x18") G1_ALIAS("RoomC0_Cmd02_ptmf", "g1_0042E390", "0x20") G1_ALIAS("D_0042E3B4", "g1_0042E390", "0x24") G1_ALIAS("D_0042E3B8", "g1_0042E390", "0x28") G1_ALIAS("RoomC0_Cmd03_ptmf", "g1_0042E390", "0x30") G1_ALIAS("D_0042E3C4", "g1_0042E390", "0x34") G1_ALIAS("D_0042E3C8", "g1_0042E390", "0x38") G1_ALIAS("RoomC0_Cmd04_ptmf", "g1_0042E390", "0x40") G1_ALIAS("D_0042E3D4", "g1_0042E390", "0x44") G1_ALIAS("D_0042E3D8", "g1_0042E390", "0x48") G1_ALIAS("RoomC0_Cond00_ptmf", "g1_0042E390", "0x50") G1_ALIAS("D_0042E3E8", "g1_0042E390", "0x58"));

/* 0x0042E428: Movie_StateStart_ptmf7, D_0042E430 */
void *g1_0042E428[6] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Movie_StateStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Movie_StateStart_ptmf7", "g1_0042E428", "0x0") G1_ALIAS("D_0042E430", "g1_0042E428", "0x8"));

/* 0x0042E500: Kind27_StateNone_ptmf, D_0042E508, Kind27_StartTurnToFiona_ptmf, D_0042E514, D_0042E518, Kind27_StartLunge_ptmf ... */
void *g1_0042E500[16] __attribute__((aligned(128), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind27_StateNone, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind27_StartTurnToFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind27_StartLunge, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind27_StartGrab, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Kind27_StateNone_ptmf", "g1_0042E500", "0x0") G1_ALIAS("D_0042E508", "g1_0042E500", "0x8") G1_ALIAS("Kind27_StartTurnToFiona_ptmf", "g1_0042E500", "0x10") G1_ALIAS("D_0042E514", "g1_0042E500", "0x14") G1_ALIAS("D_0042E518", "g1_0042E500", "0x18") G1_ALIAS("Kind27_StartLunge_ptmf", "g1_0042E500", "0x20") G1_ALIAS("D_0042E524", "g1_0042E500", "0x24") G1_ALIAS("D_0042E528", "g1_0042E500", "0x28") G1_ALIAS("Kind27_StartGrab_ptmf", "g1_0042E500", "0x30") G1_ALIAS("D_0042E534", "g1_0042E500", "0x34") G1_ALIAS("D_0042E538", "g1_0042E500", "0x38"));

/* 0x0042F180: Kind27_StateTurnToFiona_ptmf, D_0042F188, Kind27_StateBlow_ptmf, D_0042F198, Kind27_StateStun_ptmf, D_0042F1A8 ... */
void *g1_0042F180[28] __attribute__((aligned(128), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind27_StateTurnToFiona, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind27_StateBlow, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind27_StateStun, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind27_StateStun, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind27_StateLunge, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_AttackNextStep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind27_StateGrab, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Kind27_StateTurnToFiona_ptmf", "g1_0042F180", "0x0") G1_ALIAS("D_0042F188", "g1_0042F180", "0x8") G1_ALIAS("Kind27_StateBlow_ptmf", "g1_0042F180", "0x10") G1_ALIAS("D_0042F198", "g1_0042F180", "0x18") G1_ALIAS("Kind27_StateStun_ptmf", "g1_0042F180", "0x20") G1_ALIAS("D_0042F1A8", "g1_0042F180", "0x28") G1_ALIAS("Kind27_StateStun_ptmf2", "g1_0042F180", "0x30") G1_ALIAS("D_0042F1B8", "g1_0042F180", "0x38") G1_ALIAS("Kind27_StateLunge_ptmf", "g1_0042F180", "0x40") G1_ALIAS("D_0042F1C8", "g1_0042F180", "0x48") G1_ALIAS("Pursuer_AttackNextStep_ptmf8", "g1_0042F180", "0x50") G1_ALIAS("D_0042F1D8", "g1_0042F180", "0x58") G1_ALIAS("Kind27_StateGrab_ptmf", "g1_0042F180", "0x60") G1_ALIAS("D_0042F1E8", "g1_0042F180", "0x68"));

/* 0x0042F210: Thing03_StateRolling_ptmf, D_0042F218, Thing03_StateDropping_ptmf, D_0042F228 */
void *g1_0042F210[8] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Thing03_StateRolling, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Thing03_StateDropping, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Thing03_StateRolling_ptmf", "g1_0042F210", "0x0") G1_ALIAS("D_0042F218", "g1_0042F210", "0x8") G1_ALIAS("Thing03_StateDropping_ptmf", "g1_0042F210", "0x10") G1_ALIAS("D_0042F228", "g1_0042F210", "0x18"));

/* 0x0042F240: Thing06_StateDropping_ptmf, D_0042F248 */
void *g1_0042F240[4] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Thing06_StateDropping, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Thing06_StateDropping_ptmf", "g1_0042F240", "0x0") G1_ALIAS("D_0042F248", "g1_0042F240", "0x8"));

/* 0x0042F260: Thing04_StateDropping_ptmf, D_0042F268 */
void *g1_0042F260[4] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Thing04_StateDropping, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Thing04_StateDropping_ptmf", "g1_0042F260", "0x0") G1_ALIAS("D_0042F268", "g1_0042F260", "0x8"));

/* 0x0042F4F0: Pursuer_StateRunThenNext_ptmf19, D_0042F4F8 */
void *g1_0042F4F0[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf19", "g1_0042F4F0", "0x0") G1_ALIAS("D_0042F4F8", "g1_0042F4F0", "0x8"));

/* 0x00430760: RoomC7_Cmd00_ptmf, D_00430768, RoomC7_Cmd01_ptmf, D_00430774, D_00430778, RoomC7_Cmd02_ptmf ... */
void *g1_00430760[36] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC7_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC7_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC7_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC7_Cmd03, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC7_Cmd04, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC7_Cmd05, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC7_Cmd06, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC7_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC7_Cond01, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomC7_Cmd00_ptmf", "g1_00430760", "0x0") G1_ALIAS("D_00430768", "g1_00430760", "0x8") G1_ALIAS("RoomC7_Cmd01_ptmf", "g1_00430760", "0x10") G1_ALIAS("D_00430774", "g1_00430760", "0x14") G1_ALIAS("D_00430778", "g1_00430760", "0x18") G1_ALIAS("RoomC7_Cmd02_ptmf", "g1_00430760", "0x20") G1_ALIAS("D_00430784", "g1_00430760", "0x24") G1_ALIAS("D_00430788", "g1_00430760", "0x28") G1_ALIAS("RoomC7_Cmd03_ptmf", "g1_00430760", "0x30") G1_ALIAS("D_00430794", "g1_00430760", "0x34") G1_ALIAS("D_00430798", "g1_00430760", "0x38") G1_ALIAS("RoomC7_Cmd04_ptmf", "g1_00430760", "0x40") G1_ALIAS("D_004307A4", "g1_00430760", "0x44") G1_ALIAS("D_004307A8", "g1_00430760", "0x48") G1_ALIAS("RoomC7_Cmd05_ptmf", "g1_00430760", "0x50") G1_ALIAS("D_004307B4", "g1_00430760", "0x54") G1_ALIAS("D_004307B8", "g1_00430760", "0x58") G1_ALIAS("RoomC7_Cmd06_ptmf", "g1_00430760", "0x60") G1_ALIAS("D_004307C4", "g1_00430760", "0x64") G1_ALIAS("D_004307C8", "g1_00430760", "0x68") G1_ALIAS("RoomC7_Cond00_ptmf", "g1_00430760", "0x70") G1_ALIAS("D_004307D8", "g1_00430760", "0x78") G1_ALIAS("RoomC7_Cond01_ptmf", "g1_00430760", "0x80") G1_ALIAS("D_004307E4", "g1_00430760", "0x84") G1_ALIAS("D_004307E8", "g1_00430760", "0x88"));

/* 0x004308A0: Pursuer_StateRunThenNext_ptmf20, D_004308A8 */
void *g1_004308A0[4] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf20", "g1_004308A0", "0x0") G1_ALIAS("D_004308A8", "g1_004308A0", "0x8"));

/* 0x00430930: Pursuer_StateRunThenNext_ptmf21, D_00430938 */
void *g1_00430930[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf21", "g1_00430930", "0x0") G1_ALIAS("D_00430938", "g1_00430930", "0x8"));

/* 0x004309C0: Pursuer_StateRunThenNext_ptmf22, D_004309C8 */
void *g1_004309C0[4] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf22", "g1_004309C0", "0x0") G1_ALIAS("D_004309C8", "g1_004309C0", "0x8"));

/* 0x00430A90: Pursuer_StateRunThenNext_ptmf23, D_00430A98, D_00430AA0, D_00430AA8 */
void *g1_00430A90[8] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateIdle, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf23", "g1_00430A90", "0x0") G1_ALIAS("D_00430A98", "g1_00430A90", "0x8") G1_ALIAS("D_00430AA0", "g1_00430A90", "0x10") G1_ALIAS("D_00430AA8", "g1_00430A90", "0x18"));

/* 0x004326B8: Room84_Cmd00_ptmf, D_004326C0 */
void *g1_004326B8[6] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room84_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room84_Cmd00_ptmf", "g1_004326B8", "0x0") G1_ALIAS("D_004326C0", "g1_004326B8", "0x8"));

/* 0x00432AF8: Room85_Cmd00_ptmf, D_00432B00 */
void *g1_00432AF8[6] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room85_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room85_Cmd00_ptmf", "g1_00432AF8", "0x0") G1_ALIAS("D_00432B00", "g1_00432AF8", "0x8"));

/* 0x00432C90: Room87_Cmd00_ptmf, D_00432C98 */
void *g1_00432C90[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room87_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room87_Cmd00_ptmf", "g1_00432C90", "0x0") G1_ALIAS("D_00432C98", "g1_00432C90", "0x8"));

/* 0x00433550: Room88_Cmd00_ptmf, D_00433558 */
void *g1_00433550[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room88_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room88_Cmd00_ptmf", "g1_00433550", "0x0") G1_ALIAS("D_00433558", "g1_00433550", "0x8"));

/* 0x00434818: Room8C_Cmd00_ptmf, D_00434820, Room8C_Cmd01_ptmf, D_0043482C, D_00434830, Room8C_Cmd02_ptmf ... */
void *g1_00434818[22] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room8C_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room8C_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room8C_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room8C_Cmd03, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room8C_Cmd04, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room8C_Cmd00_ptmf", "g1_00434818", "0x0") G1_ALIAS("D_00434820", "g1_00434818", "0x8") G1_ALIAS("Room8C_Cmd01_ptmf", "g1_00434818", "0x10") G1_ALIAS("D_0043482C", "g1_00434818", "0x14") G1_ALIAS("D_00434830", "g1_00434818", "0x18") G1_ALIAS("Room8C_Cmd02_ptmf", "g1_00434818", "0x20") G1_ALIAS("D_0043483C", "g1_00434818", "0x24") G1_ALIAS("D_00434840", "g1_00434818", "0x28") G1_ALIAS("Room8C_Cmd03_ptmf", "g1_00434818", "0x30") G1_ALIAS("D_0043484C", "g1_00434818", "0x34") G1_ALIAS("D_00434850", "g1_00434818", "0x38") G1_ALIAS("Room8C_Cmd04_ptmf", "g1_00434818", "0x40") G1_ALIAS("D_0043485C", "g1_00434818", "0x44") G1_ALIAS("D_00434860", "g1_00434818", "0x48"));

/* 0x00435008: Room8D_Cmd00_ptmf, D_00435010, Room8D_Cmd01_ptmf, D_0043501C, D_00435020, Room8D_Cmd02_ptmf ... */
void *g1_00435008[14] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room8D_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room8D_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room8D_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room8D_Cmd00_ptmf", "g1_00435008", "0x0") G1_ALIAS("D_00435010", "g1_00435008", "0x8") G1_ALIAS("Room8D_Cmd01_ptmf", "g1_00435008", "0x10") G1_ALIAS("D_0043501C", "g1_00435008", "0x14") G1_ALIAS("D_00435020", "g1_00435008", "0x18") G1_ALIAS("Room8D_Cmd02_ptmf", "g1_00435008", "0x20") G1_ALIAS("D_0043502C", "g1_00435008", "0x24") G1_ALIAS("D_00435030", "g1_00435008", "0x28"));

/* 0x00435950: Room8E_Cmd00_ptmf, D_00435958, Room8E_Cmd01_ptmf, D_00435964, D_00435968 */
void *g1_00435950[8] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room8E_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room8E_Cmd01, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room8E_Cmd00_ptmf", "g1_00435950", "0x0") G1_ALIAS("D_00435958", "g1_00435950", "0x8") G1_ALIAS("Room8E_Cmd01_ptmf", "g1_00435950", "0x10") G1_ALIAS("D_00435964", "g1_00435950", "0x14") G1_ALIAS("D_00435968", "g1_00435950", "0x18"));

/* 0x00436568: Room8F_Cmd00_ptmf, D_00436570, Room8F_Cmd01_ptmf, D_0043657C, D_00436580, Room8F_Cmd02_ptmf ... */
void *g1_00436568[22] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room8F_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room8F_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room8F_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room8F_Cmd03, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room8F_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room8F_Cmd00_ptmf", "g1_00436568", "0x0") G1_ALIAS("D_00436570", "g1_00436568", "0x8") G1_ALIAS("Room8F_Cmd01_ptmf", "g1_00436568", "0x10") G1_ALIAS("D_0043657C", "g1_00436568", "0x14") G1_ALIAS("D_00436580", "g1_00436568", "0x18") G1_ALIAS("Room8F_Cmd02_ptmf", "g1_00436568", "0x20") G1_ALIAS("D_0043658C", "g1_00436568", "0x24") G1_ALIAS("D_00436590", "g1_00436568", "0x28") G1_ALIAS("Room8F_Cmd03_ptmf", "g1_00436568", "0x30") G1_ALIAS("D_0043659C", "g1_00436568", "0x34") G1_ALIAS("D_004365A0", "g1_00436568", "0x38") G1_ALIAS("Room8F_Cond00_ptmf", "g1_00436568", "0x40") G1_ALIAS("D_004365B0", "g1_00436568", "0x48"));

/* 0x00436CA0: Room91_Cmd00_ptmf, D_00436CA8, Room91_Cmd01_ptmf, D_00436CB4, D_00436CB8, Room91_Cmd02_ptmf ... */
void *g1_00436CA0[16] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room91_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room91_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room91_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room91_Cmd03, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room91_Cmd00_ptmf", "g1_00436CA0", "0x0") G1_ALIAS("D_00436CA8", "g1_00436CA0", "0x8") G1_ALIAS("Room91_Cmd01_ptmf", "g1_00436CA0", "0x10") G1_ALIAS("D_00436CB4", "g1_00436CA0", "0x14") G1_ALIAS("D_00436CB8", "g1_00436CA0", "0x18") G1_ALIAS("Room91_Cmd02_ptmf", "g1_00436CA0", "0x20") G1_ALIAS("D_00436CC4", "g1_00436CA0", "0x24") G1_ALIAS("D_00436CC8", "g1_00436CA0", "0x28") G1_ALIAS("Room91_Cmd03_ptmf", "g1_00436CA0", "0x30") G1_ALIAS("D_00436CD4", "g1_00436CA0", "0x34") G1_ALIAS("D_00436CD8", "g1_00436CA0", "0x38"));

/* 0x00437C40: Room92_Cmd00_ptmf, D_00437C48, Room92_Cmd01_ptmf, D_00437C54, D_00437C58, Room92_Cmd02_ptmf ... */
void *g1_00437C40[56] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room92_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room92_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room92_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room92_Cmd03, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room92_Cmd04, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room92_Cmd05, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room92_Cmd06, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room92_Cmd07, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room92_Cmd08, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room92_Cmd09, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room92_Cmd10, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room92_Cmd11, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room92_Cmd12, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room92_Cmd13, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room92_Cmd00_ptmf", "g1_00437C40", "0x0") G1_ALIAS("D_00437C48", "g1_00437C40", "0x8") G1_ALIAS("Room92_Cmd01_ptmf", "g1_00437C40", "0x10") G1_ALIAS("D_00437C54", "g1_00437C40", "0x14") G1_ALIAS("D_00437C58", "g1_00437C40", "0x18") G1_ALIAS("Room92_Cmd02_ptmf", "g1_00437C40", "0x20") G1_ALIAS("D_00437C64", "g1_00437C40", "0x24") G1_ALIAS("D_00437C68", "g1_00437C40", "0x28") G1_ALIAS("Room92_Cmd03_ptmf", "g1_00437C40", "0x30") G1_ALIAS("D_00437C74", "g1_00437C40", "0x34") G1_ALIAS("D_00437C78", "g1_00437C40", "0x38") G1_ALIAS("Room92_Cmd04_ptmf", "g1_00437C40", "0x40") G1_ALIAS("D_00437C84", "g1_00437C40", "0x44") G1_ALIAS("D_00437C88", "g1_00437C40", "0x48") G1_ALIAS("Room92_Cmd05_ptmf", "g1_00437C40", "0x50") G1_ALIAS("D_00437C94", "g1_00437C40", "0x54") G1_ALIAS("D_00437C98", "g1_00437C40", "0x58") G1_ALIAS("Room92_Cmd06_ptmf", "g1_00437C40", "0x60") G1_ALIAS("D_00437CA4", "g1_00437C40", "0x64") G1_ALIAS("D_00437CA8", "g1_00437C40", "0x68") G1_ALIAS("Room92_Cmd07_ptmf", "g1_00437C40", "0x70") G1_ALIAS("D_00437CB4", "g1_00437C40", "0x74") G1_ALIAS("D_00437CB8", "g1_00437C40", "0x78") G1_ALIAS("Room92_Cmd08_ptmf", "g1_00437C40", "0x80") G1_ALIAS("D_00437CC4", "g1_00437C40", "0x84") G1_ALIAS("D_00437CC8", "g1_00437C40", "0x88") G1_ALIAS("Room92_Cmd09_ptmf", "g1_00437C40", "0x90") G1_ALIAS("D_00437CD4", "g1_00437C40", "0x94") G1_ALIAS("D_00437CD8", "g1_00437C40", "0x98") G1_ALIAS("Room92_Cmd10_ptmf", "g1_00437C40", "0xA0") G1_ALIAS("D_00437CE4", "g1_00437C40", "0xA4") G1_ALIAS("D_00437CE8", "g1_00437C40", "0xA8") G1_ALIAS("Room92_Cmd11_ptmf", "g1_00437C40", "0xB0") G1_ALIAS("D_00437CF4", "g1_00437C40", "0xB4") G1_ALIAS("D_00437CF8", "g1_00437C40", "0xB8") G1_ALIAS("Room92_Cmd12_ptmf", "g1_00437C40", "0xC0") G1_ALIAS("D_00437D04", "g1_00437C40", "0xC4") G1_ALIAS("D_00437D08", "g1_00437C40", "0xC8") G1_ALIAS("Room92_Cmd13_ptmf", "g1_00437C40", "0xD0") G1_ALIAS("D_00437D14", "g1_00437C40", "0xD4") G1_ALIAS("D_00437D18", "g1_00437C40", "0xD8"));

/* 0x00437E30: Room97_Cmd00_ptmf, D_00437E38 */
void *g1_00437E30[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room97_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room97_Cmd00_ptmf", "g1_00437E30", "0x0") G1_ALIAS("D_00437E38", "g1_00437E30", "0x8"));

/* 0x004386F0: Room69_Cmd00_ptmf, D_004386F8 */
void *g1_004386F0[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room69_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room69_Cmd00_ptmf", "g1_004386F0", "0x0") G1_ALIAS("D_004386F8", "g1_004386F0", "0x8"));

/* 0x00438CD8: Room6A_Cmd00_ptmf, D_00438CE0, Room6A_Cmd01_ptmf, D_00438CEC, D_00438CF0 */
void *g1_00438CD8[8] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room6A_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room6A_Cmd01, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room6A_Cmd00_ptmf", "g1_00438CD8", "0x0") G1_ALIAS("D_00438CE0", "g1_00438CD8", "0x8") G1_ALIAS("Room6A_Cmd01_ptmf", "g1_00438CD8", "0x10") G1_ALIAS("D_00438CEC", "g1_00438CD8", "0x14") G1_ALIAS("D_00438CF0", "g1_00438CD8", "0x18"));

/* 0x00439AD0: Room6C_Cond00_ptmf, D_00439AD8, Room6C_Cond01_ptmf, D_00439AE4, D_00439AE8 */
void *g1_00439AD0[8] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room6C_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room6C_Cond01, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room6C_Cond00_ptmf", "g1_00439AD0", "0x0") G1_ALIAS("D_00439AD8", "g1_00439AD0", "0x8") G1_ALIAS("Room6C_Cond01_ptmf", "g1_00439AD0", "0x10") G1_ALIAS("D_00439AE4", "g1_00439AD0", "0x14") G1_ALIAS("D_00439AE8", "g1_00439AD0", "0x18"));

/* 0x0043A0B8: Room6D_Cond00_ptmf, D_0043A0C0, Room6D_Cond01_ptmf, D_0043A0CC, D_0043A0D0 */
void *g1_0043A0B8[10] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room6D_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room6D_Cond01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room6D_Cond00_ptmf", "g1_0043A0B8", "0x0") G1_ALIAS("D_0043A0C0", "g1_0043A0B8", "0x8") G1_ALIAS("Room6D_Cond01_ptmf", "g1_0043A0B8", "0x10") G1_ALIAS("D_0043A0CC", "g1_0043A0B8", "0x14") G1_ALIAS("D_0043A0D0", "g1_0043A0B8", "0x18"));

/* 0x0043AFF0: Room6F_Cmd00_ptmf, D_0043AFF8, Room6F_Cond00_ptmf, D_0043B008 */
void *g1_0043AFF0[8] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room6F_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room6F_Cond00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room6F_Cmd00_ptmf", "g1_0043AFF0", "0x0") G1_ALIAS("D_0043AFF8", "g1_0043AFF0", "0x8") G1_ALIAS("Room6F_Cond00_ptmf", "g1_0043AFF0", "0x10") G1_ALIAS("D_0043B008", "g1_0043AFF0", "0x18"));

/* 0x0043B630: Pursuer_StateRunThenNext_ptmf24, D_0043B638 */
void *g1_0043B630[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf24", "g1_0043B630", "0x0") G1_ALIAS("D_0043B638", "g1_0043B630", "0x8"));

/* 0x0043EC68: Room80_Cmd00_ptmf, D_0043EC70 */
void *g1_0043EC68[6] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room80_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room80_Cmd00_ptmf", "g1_0043EC68", "0x0") G1_ALIAS("D_0043EC70", "g1_0043EC68", "0x8"));

/* 0x0043F098: RoomC1_Cond00_ptmf, D_0043F0A0 */
void *g1_0043F098[6] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC1_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomC1_Cond00_ptmf", "g1_0043F098", "0x0") G1_ALIAS("D_0043F0A0", "g1_0043F098", "0x8"));

/* 0x0043F898: RoomC2_Cmd00_ptmf, D_0043F8A0, RoomC2_Cmd01_ptmf, D_0043F8AC, D_0043F8B0, RoomC2_Cond00_ptmf ... */
void *g1_0043F898[14] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC2_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC2_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC2_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomC2_Cmd00_ptmf", "g1_0043F898", "0x0") G1_ALIAS("D_0043F8A0", "g1_0043F898", "0x8") G1_ALIAS("RoomC2_Cmd01_ptmf", "g1_0043F898", "0x10") G1_ALIAS("D_0043F8AC", "g1_0043F898", "0x14") G1_ALIAS("D_0043F8B0", "g1_0043F898", "0x18") G1_ALIAS("RoomC2_Cond00_ptmf", "g1_0043F898", "0x20") G1_ALIAS("D_0043F8C0", "g1_0043F898", "0x28"));

/* 0x00440098: RoomC3_Cmd00_ptmf, D_004400A0, RoomC3_Cmd01_ptmf, D_004400AC, D_004400B0, RoomC3_Cmd02_ptmf ... */
void *g1_00440098[18] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC3_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC3_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC3_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC3_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomC3_Cmd00_ptmf", "g1_00440098", "0x0") G1_ALIAS("D_004400A0", "g1_00440098", "0x8") G1_ALIAS("RoomC3_Cmd01_ptmf", "g1_00440098", "0x10") G1_ALIAS("D_004400AC", "g1_00440098", "0x14") G1_ALIAS("D_004400B0", "g1_00440098", "0x18") G1_ALIAS("RoomC3_Cmd02_ptmf", "g1_00440098", "0x20") G1_ALIAS("D_004400BC", "g1_00440098", "0x24") G1_ALIAS("D_004400C0", "g1_00440098", "0x28") G1_ALIAS("RoomC3_Cond00_ptmf", "g1_00440098", "0x30") G1_ALIAS("D_004400D0", "g1_00440098", "0x38"));

/* 0x00441128: RoomC5_Cmd00_ptmf, D_00441130 */
void *g1_00441128[6] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC5_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomC5_Cmd00_ptmf", "g1_00441128", "0x0") G1_ALIAS("D_00441130", "g1_00441128", "0x8"));

/* 0x004417B8: RoomC8_Cmd00_ptmf, D_004417C0 */
void *g1_004417B8[6] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomC8_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomC8_Cmd00_ptmf", "g1_004417B8", "0x0") G1_ALIAS("D_004417C0", "g1_004417B8", "0x8"));

/* 0x004428F8: Kind37_StateBehaviour_ptmf, D_00442900 */
void *g1_004428F8[4] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind37_StateBehaviour, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Kind37_StateBehaviour_ptmf", "g1_004428F8", "0x0") G1_ALIAS("D_00442900", "g1_004428F8", "0x8"));

/* 0x00442918: Kind37_StateChase_ptmf, D_00442920 */
void *g1_00442918[4] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind37_StateChase, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Kind37_StateChase_ptmf", "g1_00442918", "0x0") G1_ALIAS("D_00442920", "g1_00442918", "0x8"));

/* 0x00443060: Room98_SlamShake_ptmf, D_00443068 */
void *g1_00443060[4] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room98_SlamShake, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room98_SlamShake_ptmf", "g1_00443060", "0x0") G1_ALIAS("D_00443068", "g1_00443060", "0x8"));

/* 0x00443550: Pursuer_StateRunThenNext_ptmf25, D_00443558 */
void *g1_00443550[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Pursuer_StateRunThenNext, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Pursuer_StateRunThenNext_ptmf25", "g1_00443550", "0x0") G1_ALIAS("D_00443558", "g1_00443550", "0x8"));

/* 0x00443718: Room99_SlamShake_ptmf, D_00443720 */
void *g1_00443718[4] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room99_SlamShake, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room99_SlamShake_ptmf", "g1_00443718", "0x0") G1_ALIAS("D_00443720", "g1_00443718", "0x8"));

/* 0x00443850: Room9A_Cmd00_ptmf, D_00443858 */
void *g1_00443850[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room9A_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room9A_Cmd00_ptmf", "g1_00443850", "0x0") G1_ALIAS("D_00443858", "g1_00443850", "0x8"));

/* 0x00443DF0: Room9B_Cmd00_ptmf, D_00443DF8 */
void *g1_00443DF0[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room9B_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room9B_Cmd00_ptmf", "g1_00443DF0", "0x0") G1_ALIAS("D_00443DF8", "g1_00443DF0", "0x8"));

/* 0x00443E40: Thing02_StateRolling_ptmf, D_00443E48, Thing02_StateDropping_ptmf, D_00443E58 */
void *g1_00443E40[8] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Thing02_StateRolling, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Thing02_StateDropping, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Thing02_StateRolling_ptmf", "g1_00443E40", "0x0") G1_ALIAS("D_00443E48", "g1_00443E40", "0x8") G1_ALIAS("Thing02_StateDropping_ptmf", "g1_00443E40", "0x10") G1_ALIAS("D_00443E58", "g1_00443E40", "0x18"));

/* 0x00444378: Room93_SlamShake_ptmf, D_00444380 */
void *g1_00444378[6] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room93_SlamShake, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room93_SlamShake_ptmf", "g1_00444378", "0x0") G1_ALIAS("D_00444380", "g1_00444378", "0x8"));

/* 0x004444D0: Room94_Cmd00_ptmf, D_004444D8 */
void *g1_004444D0[4] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room94_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room94_Cmd00_ptmf", "g1_004444D0", "0x0") G1_ALIAS("D_004444D8", "g1_004444D0", "0x8"));

/* 0x004446C8: Room95_Cmd00_ptmf, D_004446D0 */
void *g1_004446C8[6] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room95_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room95_Cmd00_ptmf", "g1_004446C8", "0x0") G1_ALIAS("D_004446D0", "g1_004446C8", "0x8"));

/* 0x00444840: Room96_Cmd00_ptmf, D_00444848 */
void *g1_00444840[4] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room96_Cmd00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room96_Cmd00_ptmf", "g1_00444840", "0x0") G1_ALIAS("D_00444848", "g1_00444840", "0x8"));

/* 0x00444CB0: Kind39_StartSink_ptmf, D_00444CB8, Kind39_StartStalkBelow_ptmf, D_00444CC4, D_00444CC8, Kind39_StartGrab_ptmf ... */
void *g1_00444CB0[12] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind39_StartSink, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind39_StartStalkBelow, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind39_StartGrab, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Kind39_StartSink_ptmf", "g1_00444CB0", "0x0") G1_ALIAS("D_00444CB8", "g1_00444CB0", "0x8") G1_ALIAS("Kind39_StartStalkBelow_ptmf", "g1_00444CB0", "0x10") G1_ALIAS("D_00444CC4", "g1_00444CB0", "0x14") G1_ALIAS("D_00444CC8", "g1_00444CB0", "0x18") G1_ALIAS("Kind39_StartGrab_ptmf", "g1_00444CB0", "0x20") G1_ALIAS("D_00444CD4", "g1_00444CB0", "0x24") G1_ALIAS("D_00444CD8", "g1_00444CB0", "0x28"));

/* 0x00445A58: Kind39_StateSinkBehind_ptmf, D_00445A60, Kind39_StateSink_ptmf, D_00445A70, Kind39_StateUnderFloor_ptmf, D_00445A80 ... */
void *g1_00445A58[38] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind39_StateSinkBehind, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind39_StateSink, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind39_StateUnderFloor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind39_StatePlayOut, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind39_StateApproach, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind39_State4930, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind39_StateRise, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind39_StateSweep, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Kind39_StateGrab, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Kind39_StateSinkBehind_ptmf", "g1_00445A58", "0x0") G1_ALIAS("D_00445A60", "g1_00445A58", "0x8") G1_ALIAS("Kind39_StateSink_ptmf", "g1_00445A58", "0x10") G1_ALIAS("D_00445A70", "g1_00445A58", "0x18") G1_ALIAS("Kind39_StateUnderFloor_ptmf", "g1_00445A58", "0x20") G1_ALIAS("D_00445A80", "g1_00445A58", "0x28") G1_ALIAS("Kind39_StatePlayOut_ptmf", "g1_00445A58", "0x30") G1_ALIAS("D_00445A90", "g1_00445A58", "0x38") G1_ALIAS("Kind39_StateApproach_ptmf", "g1_00445A58", "0x40") G1_ALIAS("D_00445AA0", "g1_00445A58", "0x48") G1_ALIAS("Kind39_State4930_ptmf", "g1_00445A58", "0x50") G1_ALIAS("D_00445AB0", "g1_00445A58", "0x58") G1_ALIAS("Kind39_StateRise_ptmf", "g1_00445A58", "0x60") G1_ALIAS("D_00445AC0", "g1_00445A58", "0x68") G1_ALIAS("Kind39_StateSweep_ptmf", "g1_00445A58", "0x70") G1_ALIAS("D_00445AD0", "g1_00445A58", "0x78") G1_ALIAS("Kind39_StateGrab_ptmf", "g1_00445A58", "0x80") G1_ALIAS("D_00445AE0", "g1_00445A58", "0x88"));

/* 0x00445B70: Thing09_StateRolling_ptmf, D_00445B78, Thing09_StateFalling_ptmf, D_00445B88 */
void *g1_00445B70[8] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Thing09_StateRolling, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Thing09_StateFalling, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Thing09_StateRolling_ptmf", "g1_00445B70", "0x0") G1_ALIAS("D_00445B78", "g1_00445B70", "0x8") G1_ALIAS("Thing09_StateFalling_ptmf", "g1_00445B70", "0x10") G1_ALIAS("D_00445B88", "g1_00445B70", "0x18"));

/* 0x00445BA0: Thing10_StateRolling_ptmf, D_00445BA8, Thing10_StateFalling_ptmf, D_00445BB8 */
void *g1_00445BA0[8] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Thing10_StateRolling, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Thing10_StateFalling, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Thing10_StateRolling_ptmf", "g1_00445BA0", "0x0") G1_ALIAS("D_00445BA8", "g1_00445BA0", "0x8") G1_ALIAS("Thing10_StateFalling_ptmf", "g1_00445BA0", "0x10") G1_ALIAS("D_00445BB8", "g1_00445BA0", "0x18"));

/* 0x00446998: Room37_Fan_ptmf, D_004469A0 */
void *g1_00446998[4] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Room37_Fan, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Room37_Fan_ptmf", "g1_00446998", "0x0") G1_ALIAS("D_004469A0", "g1_00446998", "0x8"));

/* 0x00446BA0: RoomD0_ClockStop_ptmf, D_00446BA8, RoomD0_ClockSave_ptmf, D_00446BB4, D_00446BB8, RoomD0_ClockDrawSaved_ptmf ... */
void *g1_00446BA0[16] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD0_ClockStop, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD0_ClockSave, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD0_ClockDrawSaved, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD0_ProgressCall, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomD0_ClockStop_ptmf", "g1_00446BA0", "0x0") G1_ALIAS("D_00446BA8", "g1_00446BA0", "0x8") G1_ALIAS("RoomD0_ClockSave_ptmf", "g1_00446BA0", "0x10") G1_ALIAS("D_00446BB4", "g1_00446BA0", "0x14") G1_ALIAS("D_00446BB8", "g1_00446BA0", "0x18") G1_ALIAS("RoomD0_ClockDrawSaved_ptmf", "g1_00446BA0", "0x20") G1_ALIAS("D_00446BC4", "g1_00446BA0", "0x24") G1_ALIAS("D_00446BC8", "g1_00446BA0", "0x28") G1_ALIAS("RoomD0_ProgressCall_ptmf", "g1_00446BA0", "0x30") G1_ALIAS("D_00446BD8", "g1_00446BA0", "0x38"));

/* 0x00446D08: RoomD1_Cmd00_ptmf, D_00446D10, RoomD1_Cmd01_ptmf, D_00446D1C, D_00446D20, RoomD1_Cond00_ptmf ... */
void *g1_00446D08[14] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD1_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD1_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD1_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomD1_Cmd00_ptmf", "g1_00446D08", "0x0") G1_ALIAS("D_00446D10", "g1_00446D08", "0x8") G1_ALIAS("RoomD1_Cmd01_ptmf", "g1_00446D08", "0x10") G1_ALIAS("D_00446D1C", "g1_00446D08", "0x14") G1_ALIAS("D_00446D20", "g1_00446D08", "0x18") G1_ALIAS("RoomD1_Cond00_ptmf", "g1_00446D08", "0x20") G1_ALIAS("D_00446D30", "g1_00446D08", "0x28"));

/* 0x00446EF8: RoomD2_Cmd00_ptmf, D_00446F00, RoomD2_Cond00_ptmf, D_00446F10 */
void *g1_00446EF8[10] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD2_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD2_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomD2_Cmd00_ptmf", "g1_00446EF8", "0x0") G1_ALIAS("D_00446F00", "g1_00446EF8", "0x8") G1_ALIAS("RoomD2_Cond00_ptmf", "g1_00446EF8", "0x10") G1_ALIAS("D_00446F10", "g1_00446EF8", "0x18"));

/* 0x004470E8: RoomD3_Cmd00_ptmf, D_004470F0, RoomD3_Cond00_ptmf, D_00447100 */
void *g1_004470E8[10] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD3_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD3_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomD3_Cmd00_ptmf", "g1_004470E8", "0x0") G1_ALIAS("D_004470F0", "g1_004470E8", "0x8") G1_ALIAS("RoomD3_Cond00_ptmf", "g1_004470E8", "0x10") G1_ALIAS("D_00447100", "g1_004470E8", "0x18"));

/* 0x004472C8: RoomD4_Cmd00_ptmf, D_004472D0, RoomD4_Cond00_ptmf, D_004472E0 */
void *g1_004472C8[10] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD4_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD4_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomD4_Cmd00_ptmf", "g1_004472C8", "0x0") G1_ALIAS("D_004472D0", "g1_004472C8", "0x8") G1_ALIAS("RoomD4_Cond00_ptmf", "g1_004472C8", "0x10") G1_ALIAS("D_004472E0", "g1_004472C8", "0x18"));

/* 0x00447618: RoomD5_Cmd00_ptmf, D_00447620, RoomD5_Cmd01_ptmf, D_0044762C, D_00447630, RoomD5_Cmd02_ptmf ... */
void *g1_00447618[18] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD5_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD5_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD5_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD5_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomD5_Cmd00_ptmf", "g1_00447618", "0x0") G1_ALIAS("D_00447620", "g1_00447618", "0x8") G1_ALIAS("RoomD5_Cmd01_ptmf", "g1_00447618", "0x10") G1_ALIAS("D_0044762C", "g1_00447618", "0x14") G1_ALIAS("D_00447630", "g1_00447618", "0x18") G1_ALIAS("RoomD5_Cmd02_ptmf", "g1_00447618", "0x20") G1_ALIAS("D_0044763C", "g1_00447618", "0x24") G1_ALIAS("D_00447640", "g1_00447618", "0x28") G1_ALIAS("RoomD5_Cond00_ptmf", "g1_00447618", "0x30") G1_ALIAS("D_00447650", "g1_00447618", "0x38"));

/* 0x00447A20: RoomD6_Cmd00_ptmf, D_00447A28, RoomD6_Cmd01_ptmf, D_00447A34, D_00447A38, RoomD6_Cond00_ptmf ... */
void *g1_00447A20[12] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD6_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD6_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD6_Cond00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomD6_Cmd00_ptmf", "g1_00447A20", "0x0") G1_ALIAS("D_00447A28", "g1_00447A20", "0x8") G1_ALIAS("RoomD6_Cmd01_ptmf", "g1_00447A20", "0x10") G1_ALIAS("D_00447A34", "g1_00447A20", "0x14") G1_ALIAS("D_00447A38", "g1_00447A20", "0x18") G1_ALIAS("RoomD6_Cond00_ptmf", "g1_00447A20", "0x20") G1_ALIAS("D_00447A48", "g1_00447A20", "0x28"));

/* 0x00447B70: D_00447B70, D_00447B78, RoomD7_Cond01_ptmf, D_00447B84, D_00447B88, RoomD7_Cmd00_ptmf ... */
void *g1_00447B70[20] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD7_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD7_Cond01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD7_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD7_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD7_Cmd02, (void *)0x00000000u,
};
__asm__(G1_ALIAS("D_00447B70", "g1_00447B70", "0x0") G1_ALIAS("D_00447B78", "g1_00447B70", "0x8") G1_ALIAS("RoomD7_Cond01_ptmf", "g1_00447B70", "0x10") G1_ALIAS("D_00447B84", "g1_00447B70", "0x14") G1_ALIAS("D_00447B88", "g1_00447B70", "0x18") G1_ALIAS("RoomD7_Cmd00_ptmf", "g1_00447B70", "0x20") G1_ALIAS("D_00447B98", "g1_00447B70", "0x28") G1_ALIAS("RoomD7_Cmd01_ptmf", "g1_00447B70", "0x30") G1_ALIAS("D_00447BA4", "g1_00447B70", "0x34") G1_ALIAS("D_00447BA8", "g1_00447B70", "0x38") G1_ALIAS("RoomD7_Cmd02_ptmf", "g1_00447B70", "0x40") G1_ALIAS("D_00447BB4", "g1_00447B70", "0x44") G1_ALIAS("D_00447BB8", "g1_00447B70", "0x48"));

/* 0x00447E20: D_00447E20, D_00447E28, RoomD8_Cond01_ptmf, D_00447E34, D_00447E38, RoomD8_Cmd00_ptmf ... */
void *g1_00447E20[16] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD8_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD8_Cond01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD8_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD8_Cmd01, (void *)0x00000000u,
};
__asm__(G1_ALIAS("D_00447E20", "g1_00447E20", "0x0") G1_ALIAS("D_00447E28", "g1_00447E20", "0x8") G1_ALIAS("RoomD8_Cond01_ptmf", "g1_00447E20", "0x10") G1_ALIAS("D_00447E34", "g1_00447E20", "0x14") G1_ALIAS("D_00447E38", "g1_00447E20", "0x18") G1_ALIAS("RoomD8_Cmd00_ptmf", "g1_00447E20", "0x20") G1_ALIAS("D_00447E48", "g1_00447E20", "0x28") G1_ALIAS("RoomD8_Cmd01_ptmf", "g1_00447E20", "0x30") G1_ALIAS("D_00447E54", "g1_00447E20", "0x34") G1_ALIAS("D_00447E58", "g1_00447E20", "0x38"));

/* 0x00447F88: RoomD9_Cmd00_ptmf, D_00447F90, RoomD9_Cmd01_ptmf, D_00447F9C, D_00447FA0, RoomD9_Cmd02_ptmf ... */
void *g1_00447F88[16] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD9_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD9_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD9_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomD9_Cond00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomD9_Cmd00_ptmf", "g1_00447F88", "0x0") G1_ALIAS("D_00447F90", "g1_00447F88", "0x8") G1_ALIAS("RoomD9_Cmd01_ptmf", "g1_00447F88", "0x10") G1_ALIAS("D_00447F9C", "g1_00447F88", "0x14") G1_ALIAS("D_00447FA0", "g1_00447F88", "0x18") G1_ALIAS("RoomD9_Cmd02_ptmf", "g1_00447F88", "0x20") G1_ALIAS("D_00447FAC", "g1_00447F88", "0x24") G1_ALIAS("D_00447FB0", "g1_00447F88", "0x28") G1_ALIAS("RoomD9_Cond00_ptmf", "g1_00447F88", "0x30") G1_ALIAS("D_00447FC0", "g1_00447F88", "0x38"));

/* 0x00447FE8: D_00447FE8, D_00447FF0, D_00447FF8, D_00448000, D_00448008, D_00448010 ... */
void *g1_00447FE8[50] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneEnding_StateStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneEnding_StateStaffRoll, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneEnding_StateStaffFade, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneEnding_StateResults, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneEnding_SeqSetup, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneEnding_SeqResults, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneEnding_SeqUnlocks, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneEnding_SeqTitleMessage, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneEnding_SeqSave, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneEnding_SeqTitleMessage, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneEnding_SeqSave, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneEnding_SeqSave, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("D_00447FE8", "g1_00447FE8", "0x0") G1_ALIAS("D_00447FF0", "g1_00447FE8", "0x8") G1_ALIAS("D_00447FF8", "g1_00447FE8", "0x10") G1_ALIAS("D_00448000", "g1_00447FE8", "0x18") G1_ALIAS("D_00448008", "g1_00447FE8", "0x20") G1_ALIAS("D_00448010", "g1_00447FE8", "0x28") G1_ALIAS("D_00448018", "g1_00447FE8", "0x30") G1_ALIAS("D_00448020", "g1_00447FE8", "0x38") G1_ALIAS("D_00448028", "g1_00447FE8", "0x40") G1_ALIAS("D_00448030", "g1_00447FE8", "0x48") G1_ALIAS("D_00448038", "g1_00447FE8", "0x50") G1_ALIAS("D_00448040", "g1_00447FE8", "0x58") G1_ALIAS("D_00448048", "g1_00447FE8", "0x60") G1_ALIAS("D_00448050", "g1_00447FE8", "0x68") G1_ALIAS("D_00448058", "g1_00447FE8", "0x70") G1_ALIAS("D_00448060", "g1_00447FE8", "0x78") G1_ALIAS("D_00448068", "g1_00447FE8", "0x80") G1_ALIAS("D_00448070", "g1_00447FE8", "0x88") G1_ALIAS("D_00448078", "g1_00447FE8", "0x90") G1_ALIAS("D_00448080", "g1_00447FE8", "0x98") G1_ALIAS("D_00448088", "g1_00447FE8", "0xA0") G1_ALIAS("D_00448090", "g1_00447FE8", "0xA8") G1_ALIAS("D_00448098", "g1_00447FE8", "0xB0") G1_ALIAS("D_004480A0", "g1_00447FE8", "0xB8"));

/* 0x004484F8: RoomE0_ClockStart_ptmf, D_00448500, RoomE0_Fan_ptmf, D_0044850C, D_00448510, RoomE0_WindowLight_ptmf ... */
void *g1_004484F8[30] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE0_ClockStart, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE0_Fan, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE0_WindowLight, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE0_DoorLight, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE0_ClockDraw, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE0_ChooseExit, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE0_ProgressCall, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomE0_ClockStart_ptmf", "g1_004484F8", "0x0") G1_ALIAS("D_00448500", "g1_004484F8", "0x8") G1_ALIAS("RoomE0_Fan_ptmf", "g1_004484F8", "0x10") G1_ALIAS("D_0044850C", "g1_004484F8", "0x14") G1_ALIAS("D_00448510", "g1_004484F8", "0x18") G1_ALIAS("RoomE0_WindowLight_ptmf", "g1_004484F8", "0x20") G1_ALIAS("D_0044851C", "g1_004484F8", "0x24") G1_ALIAS("D_00448520", "g1_004484F8", "0x28") G1_ALIAS("RoomE0_DoorLight_ptmf", "g1_004484F8", "0x30") G1_ALIAS("D_0044852C", "g1_004484F8", "0x34") G1_ALIAS("D_00448530", "g1_004484F8", "0x38") G1_ALIAS("RoomE0_ClockDraw_ptmf", "g1_004484F8", "0x40") G1_ALIAS("D_0044853C", "g1_004484F8", "0x44") G1_ALIAS("D_00448540", "g1_004484F8", "0x48") G1_ALIAS("RoomE0_ChooseExit_ptmf", "g1_004484F8", "0x50") G1_ALIAS("D_0044854C", "g1_004484F8", "0x54") G1_ALIAS("D_00448550", "g1_004484F8", "0x58") G1_ALIAS("RoomE0_ProgressCall_ptmf", "g1_004484F8", "0x60") G1_ALIAS("D_00448560", "g1_004484F8", "0x68"));

/* 0x00448880: RoomE1_Cmd00_ptmf, D_00448888, RoomE1_Cond00_ptmf, D_00448898 */
void *g1_00448880[8] __attribute__((aligned(128), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE1_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE1_Cond00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomE1_Cmd00_ptmf", "g1_00448880", "0x0") G1_ALIAS("D_00448888", "g1_00448880", "0x8") G1_ALIAS("RoomE1_Cond00_ptmf", "g1_00448880", "0x10") G1_ALIAS("D_00448898", "g1_00448880", "0x18"));

/* 0x00448CE8: RoomE2_Cmd00_ptmf, D_00448CF0, RoomE2_Cond00_ptmf, D_00448D00 */
void *g1_00448CE8[10] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE2_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE2_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomE2_Cmd00_ptmf", "g1_00448CE8", "0x0") G1_ALIAS("D_00448CF0", "g1_00448CE8", "0x8") G1_ALIAS("RoomE2_Cond00_ptmf", "g1_00448CE8", "0x10") G1_ALIAS("D_00448D00", "g1_00448CE8", "0x18"));

/* 0x00449220: RoomE3_Cmd00_ptmf, D_00449228, RoomE3_Cond00_ptmf, D_00449238 */
void *g1_00449220[8] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE3_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE3_Cond00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomE3_Cmd00_ptmf", "g1_00449220", "0x0") G1_ALIAS("D_00449228", "g1_00449220", "0x8") G1_ALIAS("RoomE3_Cond00_ptmf", "g1_00449220", "0x10") G1_ALIAS("D_00449238", "g1_00449220", "0x18"));

/* 0x00449508: RoomE4_Cmd00_ptmf, D_00449510, RoomE4_Cmd01_ptmf, D_0044951C, D_00449520, RoomE4_Cond00_ptmf ... */
void *g1_00449508[14] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE4_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE4_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE4_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomE4_Cmd00_ptmf", "g1_00449508", "0x0") G1_ALIAS("D_00449510", "g1_00449508", "0x8") G1_ALIAS("RoomE4_Cmd01_ptmf", "g1_00449508", "0x10") G1_ALIAS("D_0044951C", "g1_00449508", "0x14") G1_ALIAS("D_00449520", "g1_00449508", "0x18") G1_ALIAS("RoomE4_Cond00_ptmf", "g1_00449508", "0x20") G1_ALIAS("D_00449530", "g1_00449508", "0x28"));

/* 0x004498A8: RoomE5_Cmd00_ptmf, D_004498B0, RoomE5_Cmd01_ptmf, D_004498BC, D_004498C0, RoomE5_Cond00_ptmf ... */
void *g1_004498A8[14] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE5_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE5_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE5_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomE5_Cmd00_ptmf", "g1_004498A8", "0x0") G1_ALIAS("D_004498B0", "g1_004498A8", "0x8") G1_ALIAS("RoomE5_Cmd01_ptmf", "g1_004498A8", "0x10") G1_ALIAS("D_004498BC", "g1_004498A8", "0x14") G1_ALIAS("D_004498C0", "g1_004498A8", "0x18") G1_ALIAS("RoomE5_Cond00_ptmf", "g1_004498A8", "0x20") G1_ALIAS("D_004498D0", "g1_004498A8", "0x28"));

/* 0x00449A88: RoomE6_Cmd00_ptmf, D_00449A90, RoomE6_Cmd01_ptmf, D_00449A9C, D_00449AA0, RoomE6_Cond00_ptmf ... */
void *g1_00449A88[14] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE6_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE6_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE6_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomE6_Cmd00_ptmf", "g1_00449A88", "0x0") G1_ALIAS("D_00449A90", "g1_00449A88", "0x8") G1_ALIAS("RoomE6_Cmd01_ptmf", "g1_00449A88", "0x10") G1_ALIAS("D_00449A9C", "g1_00449A88", "0x14") G1_ALIAS("D_00449AA0", "g1_00449A88", "0x18") G1_ALIAS("RoomE6_Cond00_ptmf", "g1_00449A88", "0x20") G1_ALIAS("D_00449AB0", "g1_00449A88", "0x28"));

/* 0x00449EA0: RoomE7_Cmd00_ptmf, D_00449EA8, RoomE7_Cond00_ptmf, D_00449EB8 */
void *g1_00449EA0[8] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE7_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE7_Cond00, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomE7_Cmd00_ptmf", "g1_00449EA0", "0x0") G1_ALIAS("D_00449EA8", "g1_00449EA0", "0x8") G1_ALIAS("RoomE7_Cond00_ptmf", "g1_00449EA0", "0x10") G1_ALIAS("D_00449EB8", "g1_00449EA0", "0x18"));

/* 0x0044A278: RoomE8_Cmd00_ptmf, D_0044A280, RoomE8_Cond00_ptmf, D_0044A290 */
void *g1_0044A278[10] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE8_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE8_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomE8_Cmd00_ptmf", "g1_0044A278", "0x0") G1_ALIAS("D_0044A280", "g1_0044A278", "0x8") G1_ALIAS("RoomE8_Cond00_ptmf", "g1_0044A278", "0x10") G1_ALIAS("D_0044A290", "g1_0044A278", "0x18"));

/* 0x0044A938: RoomE9_Cmd00_ptmf, D_0044A940, RoomE9_Cmd01_ptmf, D_0044A94C, D_0044A950, RoomE9_Cmd02_ptmf ... */
void *g1_0044A938[22] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE9_Cmd00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE9_Cmd01, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE9_Cmd02, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE9_Cmd03, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)RoomE9_Cond00, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("RoomE9_Cmd00_ptmf", "g1_0044A938", "0x0") G1_ALIAS("D_0044A940", "g1_0044A938", "0x8") G1_ALIAS("RoomE9_Cmd01_ptmf", "g1_0044A938", "0x10") G1_ALIAS("D_0044A94C", "g1_0044A938", "0x14") G1_ALIAS("D_0044A950", "g1_0044A938", "0x18") G1_ALIAS("RoomE9_Cmd02_ptmf", "g1_0044A938", "0x20") G1_ALIAS("D_0044A95C", "g1_0044A938", "0x24") G1_ALIAS("D_0044A960", "g1_0044A938", "0x28") G1_ALIAS("RoomE9_Cmd03_ptmf", "g1_0044A938", "0x30") G1_ALIAS("D_0044A96C", "g1_0044A938", "0x34") G1_ALIAS("D_0044A970", "g1_0044A938", "0x38") G1_ALIAS("RoomE9_Cond00_ptmf", "g1_0044A938", "0x40") G1_ALIAS("D_0044A980", "g1_0044A938", "0x48"));

/* 0x0044AE00: D_0044AE00, D_0044AE08, SlotMachine_StateChoose_ptmf2, D_0044AE18, SlotMachine_StateNothing_ptmf2, D_0044AE28 */
void *g1_0044AE00[12] __attribute__((aligned(128), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateIdle, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SlotMachine_StateChoose, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SlotMachine_StateNothing, (void *)0x00000000u,
};
__asm__(G1_ALIAS("D_0044AE00", "g1_0044AE00", "0x0") G1_ALIAS("D_0044AE08", "g1_0044AE00", "0x8") G1_ALIAS("SlotMachine_StateChoose_ptmf2", "g1_0044AE00", "0x10") G1_ALIAS("D_0044AE18", "g1_0044AE00", "0x18") G1_ALIAS("SlotMachine_StateNothing_ptmf2", "g1_0044AE00", "0x20") G1_ALIAS("D_0044AE28", "g1_0044AE00", "0x28"));

/* 0x0044AE40: sSceneBootStateLoad, D_0044AE48, sSceneBootStateSequence, D_0044AE58, sSceneBootSteps, D_0044AE68 ... */
void *g1_0044AE40[116] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneBoot_StateLoadSystem, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneBoot_StateSequence, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneBoot_StepButton, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneBoot_StepTexts, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneBoot_StepProgressive, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneBoot_StepObject, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneBoot_StepCri, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneBoot_StepDolby, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneBoot_StepCaution, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneBoot_StepCapcom, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneBoot_StateDone, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateIdle, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateType, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateChoice, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateIdle, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateWaitButton, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateIdle, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateNextPage, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StatePause, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateWaitFrames, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateIdle, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateChild, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateGlyphDelay, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateType, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateType, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateIdle, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateIdle, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateIdle, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateIdle, (void *)0x00000000u,
};
__asm__(G1_ALIAS("sSceneBootStateLoad", "g1_0044AE40", "0x0") G1_ALIAS("D_0044AE48", "g1_0044AE40", "0x8") G1_ALIAS("sSceneBootStateSequence", "g1_0044AE40", "0x10") G1_ALIAS("D_0044AE58", "g1_0044AE40", "0x18") G1_ALIAS("sSceneBootSteps", "g1_0044AE40", "0x20") G1_ALIAS("D_0044AE68", "g1_0044AE40", "0x28") G1_ALIAS("D_0044AE70", "g1_0044AE40", "0x30") G1_ALIAS("D_0044AE74", "g1_0044AE40", "0x34") G1_ALIAS("D_0044AE78", "g1_0044AE40", "0x38") G1_ALIAS("D_0044AE80", "g1_0044AE40", "0x40") G1_ALIAS("D_0044AE84", "g1_0044AE40", "0x44") G1_ALIAS("D_0044AE88", "g1_0044AE40", "0x48") G1_ALIAS("D_0044AE90", "g1_0044AE40", "0x50") G1_ALIAS("D_0044AE94", "g1_0044AE40", "0x54") G1_ALIAS("D_0044AE98", "g1_0044AE40", "0x58") G1_ALIAS("D_0044AEA0", "g1_0044AE40", "0x60") G1_ALIAS("D_0044AEA4", "g1_0044AE40", "0x64") G1_ALIAS("D_0044AEA8", "g1_0044AE40", "0x68") G1_ALIAS("D_0044AEB0", "g1_0044AE40", "0x70") G1_ALIAS("D_0044AEB4", "g1_0044AE40", "0x74") G1_ALIAS("D_0044AEB8", "g1_0044AE40", "0x78") G1_ALIAS("D_0044AEC0", "g1_0044AE40", "0x80") G1_ALIAS("D_0044AEC4", "g1_0044AE40", "0x84") G1_ALIAS("D_0044AEC8", "g1_0044AE40", "0x88") G1_ALIAS("D_0044AED0", "g1_0044AE40", "0x90") G1_ALIAS("D_0044AED4", "g1_0044AE40", "0x94") G1_ALIAS("D_0044AED8", "g1_0044AE40", "0x98") G1_ALIAS("sSceneBootStateDone", "g1_0044AE40", "0xA0") G1_ALIAS("D_0044AEE8", "g1_0044AE40", "0xA8") G1_ALIAS("D_0044AEF0", "g1_0044AE40", "0xB0") G1_ALIAS("D_0044AEF8", "g1_0044AE40", "0xB8") G1_ALIAS("D_0044AF00", "g1_0044AE40", "0xC0") G1_ALIAS("D_0044AF08", "g1_0044AE40", "0xC8") G1_ALIAS("D_0044AF10", "g1_0044AE40", "0xD0") G1_ALIAS("D_0044AF18", "g1_0044AE40", "0xD8") G1_ALIAS("D_0044AF20", "g1_0044AE40", "0xE0") G1_ALIAS("D_0044AF28", "g1_0044AE40", "0xE8") G1_ALIAS("D_0044AF30", "g1_0044AE40", "0xF0") G1_ALIAS("D_0044AF38", "g1_0044AE40", "0xF8") G1_ALIAS("D_0044AF40", "g1_0044AE40", "0x100") G1_ALIAS("D_0044AF48", "g1_0044AE40", "0x108") G1_ALIAS("D_0044AF50", "g1_0044AE40", "0x110") G1_ALIAS("D_0044AF58", "g1_0044AE40", "0x118") G1_ALIAS("D_0044AF60", "g1_0044AE40", "0x120") G1_ALIAS("D_0044AF68", "g1_0044AE40", "0x128") G1_ALIAS("D_0044AF70", "g1_0044AE40", "0x130") G1_ALIAS("D_0044AF78", "g1_0044AE40", "0x138") G1_ALIAS("D_0044AF80", "g1_0044AE40", "0x140") G1_ALIAS("D_0044AF88", "g1_0044AE40", "0x148") G1_ALIAS("D_0044AF90", "g1_0044AE40", "0x150") G1_ALIAS("D_0044AF98", "g1_0044AE40", "0x158") G1_ALIAS("D_0044AFA0", "g1_0044AE40", "0x160") G1_ALIAS("D_0044AFA8", "g1_0044AE40", "0x168") G1_ALIAS("D_0044AFB0", "g1_0044AE40", "0x170") G1_ALIAS("D_0044AFB8", "g1_0044AE40", "0x178") G1_ALIAS("D_0044AFC0", "g1_0044AE40", "0x180") G1_ALIAS("D_0044AFC8", "g1_0044AE40", "0x188") G1_ALIAS("D_0044AFD0", "g1_0044AE40", "0x190") G1_ALIAS("D_0044AFD8", "g1_0044AE40", "0x198") G1_ALIAS("D_0044AFE0", "g1_0044AE40", "0x1A0") G1_ALIAS("D_0044AFE8", "g1_0044AE40", "0x1A8") G1_ALIAS("D_0044AFF0", "g1_0044AE40", "0x1B0") G1_ALIAS("D_0044AFF8", "g1_0044AE40", "0x1B8") G1_ALIAS("D_0044B000", "g1_0044AE40", "0x1C0") G1_ALIAS("D_0044B008", "g1_0044AE40", "0x1C8"));

/* 0x0044B030: D_0044B030, D_0044B038, D_0044B040, D_0044B048, D_0044B050, D_0044B058 ... */
void *g1_0044B030[140] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Task_StateIdle, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateItems, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_DrawLoadWait, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateList, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateSave, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateWordMake, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StatePageObject, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StatePageObject, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateList, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateLoad, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Costumes_State, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateMovies, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateSave, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateExtrasMenu, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateExtras, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Results_State, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Gallery_StateMusic, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Gallery_StateArtList, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateEntries, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_DrawFadeIn, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_DrawOpening, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_DrawFadeFromBlack, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateRun, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_DrawRun, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_DrawFadeOut, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_DrawClosing, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_DrawFadeBack, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_Open, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_Open, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateItemQuestion, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateItemActions, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateMap, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateMap, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateList, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateList, (void *)0x00000000u,
};
__asm__(G1_ALIAS("D_0044B030", "g1_0044B030", "0x0") G1_ALIAS("D_0044B038", "g1_0044B030", "0x8") G1_ALIAS("D_0044B040", "g1_0044B030", "0x10") G1_ALIAS("D_0044B048", "g1_0044B030", "0x18") G1_ALIAS("D_0044B050", "g1_0044B030", "0x20") G1_ALIAS("D_0044B058", "g1_0044B030", "0x28") G1_ALIAS("D_0044B060", "g1_0044B030", "0x30") G1_ALIAS("D_0044B068", "g1_0044B030", "0x38") G1_ALIAS("D_0044B070", "g1_0044B030", "0x40") G1_ALIAS("D_0044B078", "g1_0044B030", "0x48") G1_ALIAS("D_0044B080", "g1_0044B030", "0x50") G1_ALIAS("D_0044B088", "g1_0044B030", "0x58") G1_ALIAS("D_0044B090", "g1_0044B030", "0x60") G1_ALIAS("D_0044B098", "g1_0044B030", "0x68") G1_ALIAS("D_0044B0A0", "g1_0044B030", "0x70") G1_ALIAS("D_0044B0A8", "g1_0044B030", "0x78") G1_ALIAS("D_0044B0B0", "g1_0044B030", "0x80") G1_ALIAS("D_0044B0B8", "g1_0044B030", "0x88") G1_ALIAS("D_0044B0C0", "g1_0044B030", "0x90") G1_ALIAS("D_0044B0C8", "g1_0044B030", "0x98") G1_ALIAS("D_0044B0D0", "g1_0044B030", "0xA0") G1_ALIAS("D_0044B0D8", "g1_0044B030", "0xA8") G1_ALIAS("D_0044B0E0", "g1_0044B030", "0xB0") G1_ALIAS("D_0044B0E8", "g1_0044B030", "0xB8") G1_ALIAS("D_0044B0F0", "g1_0044B030", "0xC0") G1_ALIAS("D_0044B0F8", "g1_0044B030", "0xC8") G1_ALIAS("D_0044B100", "g1_0044B030", "0xD0") G1_ALIAS("D_0044B108", "g1_0044B030", "0xD8") G1_ALIAS("D_0044B110", "g1_0044B030", "0xE0") G1_ALIAS("D_0044B118", "g1_0044B030", "0xE8") G1_ALIAS("D_0044B120", "g1_0044B030", "0xF0") G1_ALIAS("D_0044B128", "g1_0044B030", "0xF8") G1_ALIAS("D_0044B130", "g1_0044B030", "0x100") G1_ALIAS("D_0044B138", "g1_0044B030", "0x108") G1_ALIAS("D_0044B140", "g1_0044B030", "0x110") G1_ALIAS("D_0044B148", "g1_0044B030", "0x118") G1_ALIAS("D_0044B150", "g1_0044B030", "0x120") G1_ALIAS("D_0044B158", "g1_0044B030", "0x128") G1_ALIAS("D_0044B160", "g1_0044B030", "0x130") G1_ALIAS("D_0044B168", "g1_0044B030", "0x138") G1_ALIAS("D_0044B170", "g1_0044B030", "0x140") G1_ALIAS("D_0044B178", "g1_0044B030", "0x148") G1_ALIAS("D_0044B180", "g1_0044B030", "0x150") G1_ALIAS("D_0044B188", "g1_0044B030", "0x158") G1_ALIAS("SubScreen_StateRun_ptmf", "g1_0044B030", "0x160") G1_ALIAS("D_0044B198", "g1_0044B030", "0x168") G1_ALIAS("D_0044B1A0", "g1_0044B030", "0x170") G1_ALIAS("D_0044B1A8", "g1_0044B030", "0x178") G1_ALIAS("D_0044B1B0", "g1_0044B030", "0x180") G1_ALIAS("D_0044B1B8", "g1_0044B030", "0x188") G1_ALIAS("SubScreen_DrawClosing_ptmf", "g1_0044B030", "0x190") G1_ALIAS("D_0044B1C8", "g1_0044B030", "0x198") G1_ALIAS("D_0044B1D0", "g1_0044B030", "0x1A0") G1_ALIAS("D_0044B1D8", "g1_0044B030", "0x1A8") G1_ALIAS("D_0044B1E0", "g1_0044B030", "0x1B0") G1_ALIAS("D_0044B1E8", "g1_0044B030", "0x1B8") G1_ALIAS("SubScreen_Open_ptmf", "g1_0044B030", "0x1C0") G1_ALIAS("D_0044B1F8", "g1_0044B030", "0x1C8") G1_ALIAS("SubScreen_StateItemQuestion_ptmf", "g1_0044B030", "0x1D0") G1_ALIAS("D_0044B208", "g1_0044B030", "0x1D8") G1_ALIAS("SubScreen_StateItemActions_ptmf", "g1_0044B030", "0x1E0") G1_ALIAS("D_0044B218", "g1_0044B030", "0x1E8") G1_ALIAS("SubScreen_StateMap_ptmf", "g1_0044B030", "0x1F0") G1_ALIAS("D_0044B228", "g1_0044B030", "0x1F8") G1_ALIAS("SubScreen_StateMap_ptmf2", "g1_0044B030", "0x200") G1_ALIAS("D_0044B238", "g1_0044B030", "0x208") G1_ALIAS("Options_StateList_ptmf", "g1_0044B030", "0x210") G1_ALIAS("D_0044B248", "g1_0044B030", "0x218") G1_ALIAS("Options_StateList_ptmf2", "g1_0044B030", "0x220") G1_ALIAS("D_0044B258", "g1_0044B030", "0x228"));

/* 0x0044B278: SubScreen_StateWordPlate_ptmf, D_0044B280, SubScreen_StateItemQuestion_ptmf2, D_0044B290, SubScreen_StateWordPlate_ptmf2, D_0044B2A0 ... */
void *g1_0044B278[184] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateWordPlate, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateItemQuestion, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateWordPlate, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateItems, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateWordPlate, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateItems, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateItems, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateItems, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateEquipAsk, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateThrowAsk, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateItems, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateItems, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateItems, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateWordPlate, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateItems, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateItems, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateItems, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateFiles, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateFiles, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateItems, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateItems, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateList, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateList, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateMap, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateMap, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateLayout, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateVibration, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateSound, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateVolume, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StatePosition, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateDefaults, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateItems, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateItems, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateFiles, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateFiles, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateList, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateList, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateList, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateList, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateList, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateList, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateList, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateList, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateList, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateList, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Options_StateList, (void *)0x00000000u,
};
__asm__(G1_ALIAS("SubScreen_StateWordPlate_ptmf", "g1_0044B278", "0x0") G1_ALIAS("D_0044B280", "g1_0044B278", "0x8") G1_ALIAS("SubScreen_StateItemQuestion_ptmf2", "g1_0044B278", "0x10") G1_ALIAS("D_0044B290", "g1_0044B278", "0x18") G1_ALIAS("SubScreen_StateWordPlate_ptmf2", "g1_0044B278", "0x20") G1_ALIAS("D_0044B2A0", "g1_0044B278", "0x28") G1_ALIAS("SubScreen_StateItems_ptmf", "g1_0044B278", "0x30") G1_ALIAS("D_0044B2B0", "g1_0044B278", "0x38") G1_ALIAS("SubScreen_StateWordPlate_ptmf3", "g1_0044B278", "0x40") G1_ALIAS("D_0044B2C0", "g1_0044B278", "0x48") G1_ALIAS("SubScreen_StateItems_ptmf2", "g1_0044B278", "0x50") G1_ALIAS("D_0044B2D0", "g1_0044B278", "0x58") G1_ALIAS("SubScreen_StateItems_ptmf3", "g1_0044B278", "0x60") G1_ALIAS("D_0044B2E0", "g1_0044B278", "0x68") G1_ALIAS("SubScreen_StateItems_ptmf4", "g1_0044B278", "0x70") G1_ALIAS("D_0044B2F0", "g1_0044B278", "0x78") G1_ALIAS("SubScreen_StateEquipAsk_ptmf", "g1_0044B278", "0x80") G1_ALIAS("D_0044B300", "g1_0044B278", "0x88") G1_ALIAS("SubScreen_StateThrowAsk_ptmf", "g1_0044B278", "0x90") G1_ALIAS("D_0044B310", "g1_0044B278", "0x98") G1_ALIAS("SubScreen_StateItems_ptmf5", "g1_0044B278", "0xA0") G1_ALIAS("D_0044B320", "g1_0044B278", "0xA8") G1_ALIAS("SubScreen_StateItems_ptmf6", "g1_0044B278", "0xB0") G1_ALIAS("D_0044B330", "g1_0044B278", "0xB8") G1_ALIAS("SubScreen_StateItems_ptmf7", "g1_0044B278", "0xC0") G1_ALIAS("D_0044B340", "g1_0044B278", "0xC8") G1_ALIAS("SubScreen_StateWordPlate_ptmf4", "g1_0044B278", "0xD0") G1_ALIAS("D_0044B350", "g1_0044B278", "0xD8") G1_ALIAS("SubScreen_StateItems_ptmf8", "g1_0044B278", "0xE0") G1_ALIAS("D_0044B360", "g1_0044B278", "0xE8") G1_ALIAS("SubScreen_StateItems_ptmf9", "g1_0044B278", "0xF0") G1_ALIAS("D_0044B370", "g1_0044B278", "0xF8") G1_ALIAS("SubScreen_StateItems_ptmf10", "g1_0044B278", "0x100") G1_ALIAS("D_0044B380", "g1_0044B278", "0x108") G1_ALIAS("SubScreen_StateFiles_ptmf", "g1_0044B278", "0x110") G1_ALIAS("D_0044B390", "g1_0044B278", "0x118") G1_ALIAS("SubScreen_StateFiles_ptmf2", "g1_0044B278", "0x120") G1_ALIAS("D_0044B3A0", "g1_0044B278", "0x128") G1_ALIAS("SubScreen_StateItems_ptmf11", "g1_0044B278", "0x130") G1_ALIAS("D_0044B3B0", "g1_0044B278", "0x138") G1_ALIAS("SubScreen_StateItems_ptmf12", "g1_0044B278", "0x140") G1_ALIAS("D_0044B3C0", "g1_0044B278", "0x148") G1_ALIAS("Options_StateList_ptmf3", "g1_0044B278", "0x150") G1_ALIAS("D_0044B3D0", "g1_0044B278", "0x158") G1_ALIAS("Options_StateList_ptmf4", "g1_0044B278", "0x160") G1_ALIAS("D_0044B3E0", "g1_0044B278", "0x168") G1_ALIAS("SubScreen_StateMap_ptmf3", "g1_0044B278", "0x170") G1_ALIAS("D_0044B3F0", "g1_0044B278", "0x178") G1_ALIAS("SubScreen_StateMap_ptmf4", "g1_0044B278", "0x180") G1_ALIAS("D_0044B400", "g1_0044B278", "0x188") G1_ALIAS("D_0044B408", "g1_0044B278", "0x190") G1_ALIAS("D_0044B410", "g1_0044B278", "0x198") G1_ALIAS("D_0044B418", "g1_0044B278", "0x1A0") G1_ALIAS("D_0044B420", "g1_0044B278", "0x1A8") G1_ALIAS("D_0044B428", "g1_0044B278", "0x1B0") G1_ALIAS("D_0044B430", "g1_0044B278", "0x1B8") G1_ALIAS("D_0044B438", "g1_0044B278", "0x1C0") G1_ALIAS("D_0044B440", "g1_0044B278", "0x1C8") G1_ALIAS("D_0044B448", "g1_0044B278", "0x1D0") G1_ALIAS("D_0044B450", "g1_0044B278", "0x1D8") G1_ALIAS("D_0044B458", "g1_0044B278", "0x1E0") G1_ALIAS("D_0044B460", "g1_0044B278", "0x1E8") G1_ALIAS("D_0044B468", "g1_0044B278", "0x1F0") G1_ALIAS("D_0044B470", "g1_0044B278", "0x1F8") G1_ALIAS("D_0044B478", "g1_0044B278", "0x200") G1_ALIAS("D_0044B480", "g1_0044B278", "0x208") G1_ALIAS("D_0044B488", "g1_0044B278", "0x210") G1_ALIAS("D_0044B490", "g1_0044B278", "0x218") G1_ALIAS("D_0044B498", "g1_0044B278", "0x220") G1_ALIAS("D_0044B4A0", "g1_0044B278", "0x228") G1_ALIAS("D_0044B4A8", "g1_0044B278", "0x230") G1_ALIAS("D_0044B4B0", "g1_0044B278", "0x238") G1_ALIAS("D_0044B4B8", "g1_0044B278", "0x240") G1_ALIAS("D_0044B4C0", "g1_0044B278", "0x248") G1_ALIAS("D_0044B4C8", "g1_0044B278", "0x250") G1_ALIAS("D_0044B4D0", "g1_0044B278", "0x258") G1_ALIAS("D_0044B4D8", "g1_0044B278", "0x260") G1_ALIAS("D_0044B4E0", "g1_0044B278", "0x268") G1_ALIAS("D_0044B4E8", "g1_0044B278", "0x270") G1_ALIAS("D_0044B4F0", "g1_0044B278", "0x278") G1_ALIAS("D_0044B4F8", "g1_0044B278", "0x280") G1_ALIAS("D_0044B500", "g1_0044B278", "0x288") G1_ALIAS("D_0044B508", "g1_0044B278", "0x290") G1_ALIAS("D_0044B510", "g1_0044B278", "0x298") G1_ALIAS("D_0044B518", "g1_0044B278", "0x2A0") G1_ALIAS("D_0044B520", "g1_0044B278", "0x2A8") G1_ALIAS("D_0044B528", "g1_0044B278", "0x2B0") G1_ALIAS("D_0044B530", "g1_0044B278", "0x2B8") G1_ALIAS("D_0044B538", "g1_0044B278", "0x2C0") G1_ALIAS("D_0044B540", "g1_0044B278", "0x2C8") G1_ALIAS("D_0044B548", "g1_0044B278", "0x2D0") G1_ALIAS("D_0044B550", "g1_0044B278", "0x2D8"));

/* 0x0044B7A0: Gallery_StateModelChosen_ptmf, D_0044B7A8 */
void *g1_0044B7A0[4] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Gallery_StateModelChosen, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Gallery_StateModelChosen_ptmf", "g1_0044B7A0", "0x0") G1_ALIAS("D_0044B7A8", "g1_0044B7A0", "0x8"));

/* 0x0044B9B0: Gallery_StateModelLoad_ptmf, D_0044B9B8, Gallery_StateModel_ptmf, D_0044B9C8, SubScreen_StateExtras_ptmf, D_0044B9D8 ... */
void *g1_0044B9B0[16] __attribute__((aligned(16), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Gallery_StateModelLoad, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Gallery_StateModel, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateExtras, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_DrawFadeFromBlack, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Gallery_StateModelLoad_ptmf", "g1_0044B9B0", "0x0") G1_ALIAS("D_0044B9B8", "g1_0044B9B0", "0x8") G1_ALIAS("Gallery_StateModel_ptmf", "g1_0044B9B0", "0x10") G1_ALIAS("D_0044B9C8", "g1_0044B9B0", "0x18") G1_ALIAS("SubScreen_StateExtras_ptmf", "g1_0044B9B0", "0x20") G1_ALIAS("D_0044B9D8", "g1_0044B9B0", "0x28") G1_ALIAS("SubScreen_DrawFadeFromBlack_ptmf", "g1_0044B9B0", "0x30") G1_ALIAS("D_0044B9E8", "g1_0044B9B0", "0x38"));

/* 0x0044C040: SubScreen_StatePageOpen_ptmf, D_0044C048, Gallery_StatePicture_ptmf, D_0044C058, Gallery_StateArtView_ptmf, D_0044C068 ... */
void *g1_0044C040[16] __attribute__((aligned(64), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StatePageOpen, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Gallery_StatePicture, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Gallery_StateArtView, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)Gallery_StateArtList, (void *)0x00000000u,
};
__asm__(G1_ALIAS("SubScreen_StatePageOpen_ptmf", "g1_0044C040", "0x0") G1_ALIAS("D_0044C048", "g1_0044C040", "0x8") G1_ALIAS("Gallery_StatePicture_ptmf", "g1_0044C040", "0x10") G1_ALIAS("D_0044C058", "g1_0044C040", "0x18") G1_ALIAS("Gallery_StateArtView_ptmf", "g1_0044C040", "0x20") G1_ALIAS("D_0044C068", "g1_0044C040", "0x28") G1_ALIAS("Gallery_StateArtList_ptmf", "g1_0044C040", "0x30") G1_ALIAS("D_0044C078", "g1_0044C040", "0x38"));

/* 0x0044C0D8: SubScreen_StateEntryQuestion_ptmf, D_0044C0E0, SubScreen_StateQuestion_ptmf, D_0044C0F0, SubScreen_StateEntries_ptmf, D_0044C100 ... */
void *g1_0044C0D8[18] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateEntryQuestion, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateQuestion, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateEntries, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SubScreen_StateEntries, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("SubScreen_StateEntryQuestion_ptmf", "g1_0044C0D8", "0x0") G1_ALIAS("D_0044C0E0", "g1_0044C0D8", "0x8") G1_ALIAS("SubScreen_StateQuestion_ptmf", "g1_0044C0D8", "0x10") G1_ALIAS("D_0044C0F0", "g1_0044C0D8", "0x18") G1_ALIAS("SubScreen_StateEntries_ptmf", "g1_0044C0D8", "0x20") G1_ALIAS("D_0044C100", "g1_0044C0D8", "0x28") G1_ALIAS("SubScreen_StateEntries_ptmf2", "g1_0044C0D8", "0x30") G1_ALIAS("D_0044C110", "g1_0044C0D8", "0x38"));

/* 0x0044C588: SceneGame_SubPlay_ptmf */
void *g1_0044C588[4] __attribute__((aligned(8), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneGame_SubPlay, (void *)0x00000000u,
};
__asm__(G1_ALIAS("SceneGame_SubPlay_ptmf", "g1_0044C588", "0x0"));

/* 0x0044C7A0: SceneGame_SubStartRoom_ptmf, D_0044C7A4, D_0044C7A8, SceneGame_StateMain_ptmf, D_0044C7B8, SceneGame_SubWaitRoom_ptmf ... */
void *g1_0044C7A0[56] __attribute__((aligned(32), used)) = {
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneGame_SubStartRoom, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneGame_StateMain, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneGame_SubWaitRoom, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneGame_StatePlay, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneGame_SubPlay, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneGame_SubPaused, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneGame_SubTransition, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneGame_SubMoviePaused, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneGame_SubPaused, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneGame_SubSubScreen, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneGame_SubPlay, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneGame_SubPlay, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneGame_SubPlay, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0xFFFFFFFFu, (void *)SceneGame_SubPlay, (void *)0x00000000u,
};
__asm__(G1_ALIAS("SceneGame_SubStartRoom_ptmf", "g1_0044C7A0", "0x0") G1_ALIAS("D_0044C7A4", "g1_0044C7A0", "0x4") G1_ALIAS("D_0044C7A8", "g1_0044C7A0", "0x8") G1_ALIAS("SceneGame_StateMain_ptmf", "g1_0044C7A0", "0x10") G1_ALIAS("D_0044C7B8", "g1_0044C7A0", "0x18") G1_ALIAS("SceneGame_SubWaitRoom_ptmf", "g1_0044C7A0", "0x20") G1_ALIAS("D_0044C7C4", "g1_0044C7A0", "0x24") G1_ALIAS("D_0044C7C8", "g1_0044C7A0", "0x28") G1_ALIAS("SceneGame_StatePlay_ptmf", "g1_0044C7A0", "0x30") G1_ALIAS("D_0044C7D8", "g1_0044C7A0", "0x38") G1_ALIAS("SceneGame_SubPlay_ptmf2", "g1_0044C7A0", "0x40") G1_ALIAS("D_0044C7E8", "g1_0044C7A0", "0x48") G1_ALIAS("SceneGame_SubPaused_ptmf", "g1_0044C7A0", "0x50") G1_ALIAS("SceneGame_SubTransition_ptmf", "g1_0044C7A0", "0x60") G1_ALIAS("D_0044C808", "g1_0044C7A0", "0x68") G1_ALIAS("SceneGame_SubMoviePaused_ptmf", "g1_0044C7A0", "0x70") G1_ALIAS("D_0044C818", "g1_0044C7A0", "0x78") G1_ALIAS("SceneGame_SubPaused_ptmf2", "g1_0044C7A0", "0x80") G1_ALIAS("D_0044C828", "g1_0044C7A0", "0x88") G1_ALIAS("SceneGame_SubSubScreen_ptmf", "g1_0044C7A0", "0x90") G1_ALIAS("D_0044C838", "g1_0044C7A0", "0x98") G1_ALIAS("SceneGame_SubPlay_ptmf3", "g1_0044C7A0", "0xA0") G1_ALIAS("D_0044C848", "g1_0044C7A0", "0xA8") G1_ALIAS("SceneGame_SubPlay_ptmf4", "g1_0044C7A0", "0xB0") G1_ALIAS("D_0044C858", "g1_0044C7A0", "0xB8") G1_ALIAS("SceneGame_SubPlay_ptmf5", "g1_0044C7A0", "0xC0") G1_ALIAS("D_0044C868", "g1_0044C7A0", "0xC8") G1_ALIAS("SceneGame_SubPlay_ptmf6", "g1_0044C7A0", "0xD0") G1_ALIAS("D_0044C878", "g1_0044C7A0", "0xD8"));

/* 0x00469460: Sinit_Table, D_00469480, D_0046969C, D_004696A0, D_004696B0, D_004696C0 ... */
void *g1_00469460[17700] __attribute__((aligned(32), used)) = {
    (void *)Sinit_Flags, (void *)Sinit_FillTables, (void *)Sinit_Debilitas, (void *)Sinit_Renderer,
    (void *)Sinit_Game, (void *)Sinit_Pursuer, (void *)Sinit_SceneGameMembers, (void *)Sinit_Room02,
    (void *)Sinit_Room03, (void *)Sinit_Room04, (void *)Sinit_Room06, (void *)Sinit_Room08,
    (void *)Sinit_Room09, (void *)Sinit_Room0A, (void *)Sinit_Room0B, (void *)Sinit_Room0C,
    (void *)Sinit_Room0F, (void *)Sinit_Room10, (void *)Sinit_Room12, (void *)Sinit_Room13,
    (void *)Sinit_Room14, (void *)Sinit_Room15, (void *)Sinit_Room18, (void *)Sinit_Room19,
    (void *)Sinit_Room1A, (void *)Sinit_Room1B, (void *)Sinit_Room1C, (void *)Sinit_Room1D,
    (void *)Sinit_Room1F, (void *)Sinit_Room20, (void *)Sinit_Room21, (void *)Sinit_Room23,
    (void *)Sinit_Room24, (void *)Sinit_Room25, (void *)Sinit_Room26, (void *)Sinit_Room28,
    (void *)Sinit_Room29, (void *)Sinit_Room2A, (void *)Sinit_Room2B, (void *)Sinit_Room2D,
    (void *)Sinit_Room40, (void *)Sinit_Room43, (void *)Sinit_Room45, (void *)Sinit_Room49,
    (void *)Sinit_Room4B, (void *)Sinit_Room4C, (void *)Sinit_Room4E, (void *)Sinit_Creature,
    (void *)Sinit_Room50, (void *)Sinit_Room51, (void *)Sinit_Room52, (void *)Sinit_Room56,
    (void *)Sinit_Room59, (void *)Sinit_Room5A, (void *)Sinit_Room5C, (void *)Sinit_Room5D,
    (void *)Sinit_Room2F, (void *)Sinit_Room30, (void *)Sinit_Debilitas3, (void *)Sinit_Iostreams2,
    (void *)Sinit_Riccardo, (void *)Sinit_Room46, (void *)Sinit_Room102, (void *)Sinit_Room103,
    (void *)Sinit_Room104, (void *)Sinit_Room105, (void *)Sinit_Room106, (void *)Sinit_Room107,
    (void *)Sinit_Room108, (void *)Sinit_Room109, (void *)Sinit_Room10A, (void *)Sinit_Room10B,
    (void *)Sinit_Debilitas2, (void *)Sinit_Room27, (void *)Sinit_Room2E, (void *)Sinit_Room66,
    (void *)Sinit_Room55, (void *)Sinit_Room63, (void *)Sinit_Room62, (void *)Sinit_Lorenzo,
    (void *)Sinit_Room48, (void *)Sinit_Room54, (void *)Sinit_Room60, (void *)Sinit_Room61,
    (void *)Sinit_Room31, (void *)Sinit_Room32, (void *)Sinit_Room86, (void *)Sinit_RoomC0,
    (void *)Sinit_Debilitas3_2, (void *)Sinit_RoomC7, (void *)Sinit_Room84, (void *)Sinit_Room85,
    (void *)Sinit_Room87, (void *)Sinit_Room88, (void *)Sinit_Room8C, (void *)Sinit_Room8D,
    (void *)Sinit_Room8E, (void *)Sinit_Room8F, (void *)Sinit_Room91, (void *)Sinit_Room92,
    (void *)Sinit_Room97, (void *)Sinit_Room69, (void *)Sinit_Room6A, (void *)Sinit_Room6C,
    (void *)Sinit_Room6D, (void *)Sinit_Room6F, (void *)Sinit_Room80, (void *)Sinit_RoomC1,
    (void *)Sinit_RoomC2, (void *)Sinit_RoomC3, (void *)Sinit_RoomC5, (void *)Sinit_RoomC8,
    (void *)Sinit_Room98, (void *)Sinit_Room99, (void *)Sinit_Room9A, (void *)Sinit_Room9B,
    (void *)Sinit_Room93, (void *)Sinit_Room94, (void *)Sinit_Room95, (void *)Sinit_Room96,
    (void *)Sinit_Lorenzo_2, (void *)Sinit_Room37, (void *)Sinit_RoomD0, (void *)Sinit_RoomD1,
    (void *)Sinit_RoomD2, (void *)Sinit_RoomD3, (void *)Sinit_RoomD4, (void *)Sinit_RoomD5,
    (void *)Sinit_RoomD6, (void *)Sinit_RoomD7, (void *)Sinit_RoomD8, (void *)Sinit_RoomD9,
    (void *)Sinit_RoomE0, (void *)Sinit_RoomE1, (void *)Sinit_RoomE2, (void *)Sinit_RoomE3,
    (void *)Sinit_RoomE4, (void *)Sinit_RoomE5, (void *)Sinit_RoomE6, (void *)Sinit_RoomE7,
    (void *)Sinit_RoomE8, (void *)Sinit_RoomE9, (void *)Sinit_Iostreams3, (void *)0x00000000u,
    (void *)D_0044C890, (void *)0x00000000u, (void *)func_001004F0, (void *)Exception_What,
    (void *)(D_0044C8E8 + 0x10), (void *)0x00000000u, (void *)func_001005F0, (void *)BadAlloc_What,
    (void *)(D_0044CAA8 + 0x10), (void *)0x00000000u, (void *)func_00101420, (void *)BadException_What,
    (void *)D_0044CAE0, (void *)0x00000000u, (void *)func_001027C0, (void *)0x00000000u,
    (void *)(D_0044CB08 + 0x10), (void *)0x00000000u, (void *)func_00102730, (void *)func_001029B0,
    (void *)D_0044CB38, (void *)0x00000000u, (void *)func_00102CC0, (void *)0x00000000u,
    (void *)(D_0044CBF0 + 0x18), (void *)0x00000000u, (void *)func_001037C0, (void *)(D_0044CBF0 + 0x18),
    (void *)0xFFFFFFE8u, (void *)func_0010B130, (void *)D_0044CBE8, (void *)0x00000000u,
    (void *)func_00103860, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)(D_0044CC50 + 0x18), (void *)0x00000000u, (void *)func_00103720, (void *)(D_0044CC50 + 0x18),
    (void *)0xFFFFFFF8u, (void *)func_0010B120, (void *)0x00000000u, (void *)0x00000000u,
    (void *)D_0044CCB0, (void *)0x00000000u, (void *)func_001038C0, (void *)func_00104220,
    (void *)func_001028A0, (void *)func_00104210, (void *)func_00104200, (void *)func_00103A40,
    (void *)func_00103A50, (void *)func_00104020, (void *)func_00104010, (void *)func_00103FB0,
    (void *)func_00103FA0, (void *)func_00103DC0, (void *)func_00103DB0, (void *)0x00000000u,
    (void *)(D_0044CCF8 + 0x10), (void *)0x00000000u, (void *)func_001035A0, (void *)func_00105B30,
    (void *)func_00103A20, (void *)func_00105ED0, (void *)func_00105D00, (void *)func_00105C40,
    (void *)func_00106AE0, (void *)func_00104020, (void *)func_001068D0, (void *)func_00106710,
    (void *)func_00106640, (void *)func_00103DC0, (void *)func_00106160, (void *)0x00000000u,
    (void *)(D_0044CDA0 + 0x18), (void *)0x00000000u, (void *)func_00103250, (void *)(D_0044CDA0 + 0x18),
    (void *)0xFFFFFFE8u, (void *)func_0010B110, (void *)D_0044CD90, (void *)0x00000000u,
    (void *)func_00103940, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)(D_0044CE00 + 0x18), (void *)0x00000000u, (void *)func_001031B0, (void *)(D_0044CE00 + 0x18),
    (void *)0xFFFFFFF8u, (void *)func_0010B100, (void *)0x00000000u, (void *)0x00000000u,
    (void *)D_0044CE58, (void *)0x00000000u, (void *)func_001039A0, (void *)func_00103D90,
    (void *)func_001028B0, (void *)func_00103D80, (void *)func_00103D70, (void *)func_00103A60,
    (void *)func_00103A70, (void *)func_00103C40, (void *)func_00103A80, (void *)func_00103A90,
    (void *)func_00103AF0, (void *)func_00103B10, (void *)func_00103B00, (void *)0x00000000u,
    (void *)(D_0044CE98 + 0x10), (void *)0x00000000u, (void *)func_00103030, (void *)func_00104AC0,
    (void *)func_00103A30, (void *)func_00104E60, (void *)func_00104C90, (void *)func_00104BD0,
    (void *)func_001059E0, (void *)func_00103C40, (void *)func_001057E0, (void *)func_00105620,
    (void *)func_00105550, (void *)func_00103B10, (void *)func_001050E0, (void *)0x00000000u,
    (void *)(D_0044CF30 + 0x18), (void *)0x00000000u, (void *)func_00108780, (void *)func_001087E0,
    (void *)func_00108880, (void *)func_001080B0, (void *)func_00105EC0, (void *)func_00105C10,
    (void *)func_00108920, (void *)func_00108960, (void *)0x00000000u, (void *)0x00000000u,
    (void *)(D_0044CF70 + 0x18), (void *)0x00000000u, (void *)func_00108720, (void *)func_001079A0,
    (void *)func_001079B0, (void *)func_001079C0, (void *)func_00104E50, (void *)func_00104BA0,
    (void *)func_001086D0, (void *)func_001086C0, (void *)0x00000000u, (void *)0x00000000u,
    (void *)(D_0044CFD0 + 0x18), (void *)0x00000000u, (void *)func_00108DD0, (void *)func_00108EE0,
    (void *)func_00108F30, (void *)func_00108FB0, (void *)func_00109040, (void *)func_001090D0,
    (void *)func_00109100, (void *)func_00109170, (void *)func_001091A0, (void *)func_001044B0,
    (void *)func_00109210, (void *)func_00109240, (void *)func_00109270, (void *)0x00000000u,
    (void *)(D_0044D0A0 + 0x18), (void *)0x00000000u, (void *)func_00109F70, (void *)func_0010AF90,
    (void *)D_0044D098, (void *)0x00000000u, (void *)func_00109EC0, (void *)func_0010AF90,
    (void *)(D_0044D0F0 + 0x18), (void *)0x00000000u, (void *)func_0010B350, (void *)func_0010B3E0,
    (void *)func_0010B400, (void *)func_0010B440, (void *)func_0010B460, (void *)func_0010B4A0,
    (void *)func_0010B4B0, (void *)func_0010B4E0, (void *)func_0010B4F0, (void *)0x00000000u,
    (void *)(D_0044D220 + 0x18), (void *)0x00000000u, (void *)func_0010BAB0, (void *)func_0010AF90,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Scene_dtor, (void *)Scene_Update,
    (void *)0x00000000u, (void *)Scene_OnSoftReset, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)BlockPool_dtor, (void *)BlockPool_FreeAll,
    (void *)BlockPool_Alloc, (void *)BlockPool_Free, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SceneTableBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Thing_dtor, (void *)Thing_Setup,
    (void *)Actor_Cleanup, (void *)Actor_LoadFiles, (void *)Actor_FilesLoading, (void *)Actor_FilesLoaded,
    (void *)Actor_Unload, (void *)Actor_RememberPos, (void *)Actor_Place, (void *)Thing_Draw,
    (void *)Thing_Frame, (void *)Actor_LeaveScreen, (void *)Actor_ShowUp, (void *)Actor_CanAct,
    (void *)Thing_PutAt, (void *)Thing_PutDown, (void *)Thing_LandTri, (void *)Thing_MotionState,
    (void *)Obj469A00_Noop50, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Camera_dtor, (void *)Camera_Defaults,
    (void *)Camera_Reset, (void *)Camera_Update, (void *)Camera_ViewMatrix, (void *)Camera_SetEye,
    (void *)Camera_GetEye, (void *)Camera_Get60, (void *)Camera_SetTarget, (void *)Camera_GetTarget,
    (void *)Camera_MoveEye, (void *)Camera_MoveTarget, (void *)Camera_Orbit, (void *)Camera_TurnView,
    (void *)Camera_MoveLocal, (void *)Camera_GetMtx150, (void *)Camera_GetMtx190, (void *)Camera_GetMtx1D0,
    (void *)Camera_SetMtx150, (void *)Camera_SetMtx190, (void *)Camera_GetMtx210, (void *)Camera_SetFov,
    (void *)Camera_GetView, (void *)Camera_GetFov, (void *)Camera_ViewHeading, (void *)Camera_Set4,
    (void *)Camera_SetRoll, (void *)Camera_GetRoll, (void *)Camera_FollowPath, (void *)Camera_NoPath,
    (void *)Camera_PathCount, (void *)Camera_Preset, (void *)Camera_ApplyPreset, (void *)Camera_ApplyPresetArg,
    (void *)Camera_Get294, (void *)Camera_Get298, (void *)Camera_GetPath, (void *)Camera_CurrentPreset,
    (void *)Camera_GetDir, (void *)Camera_ViewAxis, (void *)Camera_SetZMax, (void *)Camera_SetZMin,
    (void *)Camera_SetNear, (void *)Camera_SetFar, (void *)Camera_ZValue, (void *)Camera_MtxTranslation,
    (void *)Camera_Set28, (void *)Camera_SetCentre, (void *)Camera_GetViewScreen, (void *)Camera_GetNear,
    (void *)Camera_GetFar, (void *)Camera_OutOfView, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)CameraBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Actor_dtor, (void *)Actor_Reset,
    (void *)Actor_Cleanup, (void *)Actor_LoadFiles, (void *)Actor_FilesLoading, (void *)Actor_FilesLoaded,
    (void *)Actor_Unload, (void *)Actor_RememberPos, (void *)Actor_Place, (void *)Actor_LightChange,
    (void *)Actor_Update, (void *)Actor_LeaveScreen, (void *)Actor_ShowUp, (void *)Actor_CanAct,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Character_dtor, (void *)Character_Reset,
    (void *)Actor_Cleanup, (void *)Actor_LoadFiles, (void *)Actor_FilesLoading, (void *)Actor_FilesLoaded,
    (void *)Actor_Unload, (void *)Character_RememberPos, (void *)Character_Place, (void *)Actor_LightChange,
    (void *)Actor_Update, (void *)Actor_LeaveScreen, (void *)Actor_ShowUp, (void *)Actor_CanAct,
    (void *)0x00000000u, (void *)Character_Think, (void *)Character_PlaceModel, (void *)Character_Disable,
    (void *)Character_Enable, (void *)Character_LoadMessage, (void *)Character_Deactivate, (void *)Character_Activate,
    (void *)Character_ResetBehaviour, (void *)0x00000000u, (void *)Character_EventConcerns, (void *)Character_SaveState,
    (void *)Character_LoadState, (void *)Character_AttackPoint, (void *)Character_LeftBehind, (void *)Character_BackToStance,
    (void *)Character_RegionFade, (void *)0x00000000u, (void *)Character_EventCommand, (void *)Character_BackToNormal,
    (void *)Character_EventReset, (void *)Character_TimerDown, (void *)Character_IsBusy, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Helper469D00_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Debilitas_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Debilitas_Update, (void *)Pursuer_LeaveScreen, (void *)Pursuer_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Pursuer_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Debilitas_DoorOffset,
    (void *)Debilitas_TurnRate, (void *)Debilitas_TurnRateFast, (void *)Debilitas_BlockFlags, (void *)Debilitas_GoTo,
    (void *)Debilitas_HeadForFiona, (void *)Debilitas_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)Debilitas_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)Debilitas_Setup, (void *)Debilitas_ModelFiles, (void *)Debilitas_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Pursuer_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Debilitas_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Debilitas_StandAnim, (void *)Pursuer_Threat,
    (void *)Debilitas_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Debilitas_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Debilitas_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Debilitas_Stairs, (void *)Debilitas_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Debilitas_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Debilitas_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Debilitas_GoForFiona,
    (void *)Debilitas_HeadingStep, (void *)Debilitas_FreshStart, (void *)Debilitas_CarryOn, (void *)Pursuer_Caught,
    (void *)Debilitas_Timer10s, (void *)Pursuer_Timer15s, (void *)Debilitas_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Debilitas_ActionOffsets, (void *)Debilitas_LookFrames,
    (void *)Debilitas_LookSwing, (void *)Debilitas_ReachFiona, (void *)Debilitas_Dist2E8, (void *)Debilitas_AttackAngle,
    (void *)Debilitas_ReachHewie, (void *)Debilitas_AttackRange, (void *)Debilitas_SpeedTop, (void *)Debilitas_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Debilitas_AttackAnimA,
    (void *)Debilitas_AttackAnimB, (void *)Debilitas_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Debilitas_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SceneTitle_dtor, (void *)Scene_Update,
    (void *)SceneTitle_StateEntry, (void *)SceneTitle_Finish, (void *)0x00000000u, (void *)0x00000000u,
    (void *)BootCard_dtor, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)TextObj_dtor, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)SubPool_dtor, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SubScreenBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)BgmCtl_Want, (void *)BgmCtl_dtor,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Hewie_dtor, (void *)Hewie_Reset,
    (void *)Hewie_Cleanup, (void *)Hewie_LoadFiles, (void *)Actor_FilesLoading, (void *)Hewie_FilesLoaded,
    (void *)Hewie_Unload, (void *)Hewie_RememberPos, (void *)Hewie_PlaceOn, (void *)Hewie_LightChange,
    (void *)Hewie_Update, (void *)Hewie_Vt34, (void *)Hewie_Vt38, (void *)Actor_CanAct,
    (void *)Hewie_AnimUpdate, (void *)Hewie_Think, (void *)Hewie_FollowAnim, (void *)Hewie_Disable,
    (void *)Hewie_Halt, (void *)Character_LoadMessage, (void *)Hewie_Deactivate, (void *)Hewie_Activate,
    (void *)Hewie_ToIdle, (void *)Hewie_PlaceInRoom, (void *)Hewie_CanInteract, (void *)Hewie_SaveState,
    (void *)Hewie_LoadState, (void *)Hewie_PointOfInterest, (void *)Hewie_LeftBehind, (void *)Hewie_BackToIdle,
    (void *)Character_RegionFade, (void *)Hewie_StateBlock, (void *)Hewie_Requests, (void *)Hewie_Interrupted,
    (void *)Hewie_FullStop, (void *)Hewie_TakeDamage, (void *)Hewie_IsBusy, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Heap_dtor, (void *)Heap_Init,
    (void *)Heap_Alloc, (void *)Heap_Free, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Loader_dtor, (void *)Loader_Queue,
    (void *)Loader_Cancel, (void *)Loader_CancelGroup, (void *)Loader_CancelGroupWait, (void *)Loader_CancelAll,
    (void *)Loader_State, (void *)Loader_Slot24, (void *)Loader_Slot28, (void *)Loader_FileSectors,
    (void *)Loader_FileSize, (void *)Loader_LoadNow, (void *)Loader_CurrentDone, (void *)Loader_DirListing,
    (void *)0x00000000u, (void *)0x00000000u, (void *)LoaderBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Progress_dtor, (void *)ProgressBase_SlotC,
    (void *)ProgressBase_Slot10, (void *)ProgressBase_Slot14, (void *)ProgressBase_Slot18, (void *)ProgressBase_Slot1C,
    (void *)ProgressBase_Slot20, (void *)ProgressBase_Slot24, (void *)ProgressBase_Slot28, (void *)Progress_CharBoneCall,
    (void *)Progress_CharNear, (void *)Progress_SlotAfresh, (void *)ProgressBase_Slot38, (void *)ProgressBase_Slot3C,
    (void *)ProgressBase_Slot40, (void *)ProgressBase_Slot44, (void *)ProgressBase_Slot48, (void *)ProgressBase_Slot4C,
    (void *)ProgressBase_Slot50, (void *)Progress_Noop54, (void *)ProgressBase_Slot58, (void *)ProgressBase_Slot5C,
    (void *)ProgressBase_Slot60, (void *)Progress_StalkerAlert, (void *)ProgressBase_Slot68, (void *)ProgressBase_Slot6C,
    (void *)ProgressBase_Slot70, (void *)Progress_Noop74, (void *)Progress_Noop78, (void *)ProgressBase_Slot7C,
    (void *)Progress_WhoIsWhere, (void *)ProgressBase_Slot84, (void *)ProgressBase_Slot88, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind15_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind14_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind14_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)Kind14_Setup, (void *)Kind14_ModelFiles, (void *)Kind14_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind14_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind22_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind21_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind21_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)func_00179AB0, (void *)Kind21_ModelFiles, (void *)Kind21_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind21_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Obj46A9B0_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Obj46A9C0_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)NavMesh_dtor, (void *)NavMesh_TriCentre,
    (void *)NavMesh_PointInTri, (void *)NavMesh_OntoPlane, (void *)NavMesh_PutOnLow, (void *)NavMesh_PutOnHigh,
    (void *)NavMesh_ExitEdge, (void *)NavMesh_ExitEdgeHit, (void *)NavMesh_SlideIn, (void *)NavMesh_TriNormal,
    (void *)NavMesh_TriSlope, (void *)NavMesh_SegmentHit, (void *)NavMesh_HeightOver, (void *)NavMesh_FindTri,
    (void *)NavMesh_Walk, (void *)NavMesh_Follow, (void *)NavMesh_OuterEdge, (void *)NavMesh_FindDoorRegions,
    (void *)NavMesh_AtDoorRegion, (void *)NavMesh_DoorTri, (void *)NavMesh_DoorFacing, (void *)NavMesh_DoorSpot,
    (void *)NavMesh_DoorSide, (void *)NavMesh_FindFrom, (void *)NavMesh_NeighbourAt, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)NavMeshBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Fiona_dtor, (void *)Fiona_Reset,
    (void *)Fiona_Cleanup, (void *)Fiona_LoadFiles, (void *)Actor_FilesLoading, (void *)Fiona_FilesLoaded,
    (void *)Fiona_Unload, (void *)Fiona_RememberPos, (void *)Fiona_PlaceOn, (void *)Fiona_LightChange,
    (void *)Fiona_Update, (void *)Fiona_Vt34, (void *)Fiona_Vt38, (void *)Actor_CanAct,
    (void *)Fiona_AnimUpdate, (void *)Fiona_Think, (void *)Fiona_FollowAnim, (void *)Fiona_Disable,
    (void *)Fiona_Halt, (void *)Fiona_LoadMessage, (void *)Fiona_Deactivate, (void *)Fiona_Activate,
    (void *)Fiona_ForgetPath, (void *)Fiona_PlaceInRoom, (void *)Fiona_CanInteract, (void *)Character_SaveState,
    (void *)Character_LoadState, (void *)Fiona_PointOfInterest, (void *)Character_LeftBehind, (void *)Fiona_BackToIdle,
    (void *)Character_RegionFade, (void *)Fiona_StateBlock, (void *)Fiona_Requests, (void *)Fiona_Interrupted,
    (void *)Fiona_FullStop, (void *)Character_TimerDown, (void *)Fiona_IsBusy, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Random_dtor, (void *)Random_Seed,
    (void *)Random_Int32, (void *)Random_Int31, (void *)Random_Real1, (void *)Random_Real2,
    (void *)Random_Real3, (void *)Random_Res53, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RandomBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)PathPlan_dtor, (void *)PathPlan_Start,
    (void *)PathPlan_Step, (void *)PathPlan_RunSearch0, (void *)PathPlan_Curve, (void *)PathPlan_Points,
    (void *)PathPlan_AdvanceOnMesh, (void *)PathPlan_Advance, (void *)BitMask_Clear, (void *)PathPlan_EndCentre,
    (void *)PathPlan_LastLength, (void *)PathPlan_CutBack, (void *)PathPlan_EndTri, (void *)PathPlan_PointsLength,
    (void *)PathPlan_PathLength, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)PathPlanBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Renderer_dtor, (void *)Renderer_Draw,
    (void *)Renderer_AddPacket, (void *)Renderer_AllocArena1, (void *)Renderer_AllocArena2, (void *)Renderer_Set304DE0,
    (void *)Renderer_PacketFor, (void *)Renderer_SetVideoMode, (void *)Renderer_VideoMode, (void *)Renderer_Display,
    (void *)Renderer_SetScreenOffset, (void *)Renderer_Set304BF8, (void *)Renderer_VramEntry, (void *)Renderer_LayerId,
    (void *)func_001BB780, (void *)Renderer_UploadTexture, (void *)Renderer_PutImage8, (void *)Renderer_PutImage4,
    (void *)func_001BAB80, (void *)func_001BA970, (void *)Renderer_Glow, (void *)Renderer_GlowClear,
    (void *)Renderer_Additive, (void *)Renderer_SetTint11, (void *)Renderer_GetTint11, (void *)Renderer_Flares,
    (void *)Renderer_SetColour54, (void *)Renderer_GetColour54, (void *)Renderer_SetSpecialColour, (void *)Renderer_Sprite,
    (void *)Renderer_DrawBox, (void *)Renderer_Quad, (void *)Renderer_QuadArgs, (void *)Renderer_SetFloorEffect,
    (void *)Renderer_FillScreen, (void *)Renderer_BuildPalette, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RendererBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)Obj46AD88_dtor, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ModelDrawer_dtor, (void *)func_001BDF80,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Pads_dtor, (void *)Pads_SetMotors,
    (void *)Pads_GetData, (void *)0x00000000u, (void *)0x00000000u, (void *)func_001BE790,
    (void *)0x00000000u, (void *)0x00000000u, (void *)PadsBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)System_dtor, (void *)System_Init,
    (void *)System_EndFrame, (void *)System_FrameNoWait, (void *)System_Shutdown, (void *)func_001BED00,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SystemBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RumbleBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MemCardBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MemCard_dtor, (void *)MemCard_Check,
    (void *)MemCard_Request10, (void *)MemCard_Write, (void *)MemCard_Read, (void *)MemCard_Request1C,
    (void *)MemCard_Request20, (void *)0x00000000u, (void *)0x00000000u, (void *)func_001BF870,
    (void *)0x00000000u, (void *)0x00000000u, (void *)AdxBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MovieLibBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MovieSys_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)func_001BF860, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)VramBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SndDriverBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Vram_dtor, (void *)Vram_Init,
    (void *)Vram_Reset, (void *)Vram_Alloc, (void *)Vram_AllocAt, (void *)Vram_Free,
    (void *)Vram_FreeAll, (void *)Vram_KeepResident, (void *)Vram_Tex0Load, (void *)Vram_Tex0Indexed,
    (void *)Vram_Tex0Csa, (void *)Vram_Tex2, (void *)Vram_EntryTex0, (void *)Vram_EntryTex0Indexed,
    (void *)Vram_EntryTex0Csa, (void *)Vram_EntryTex2, (void *)Vram_Upload, (void *)Vram_LoadTex,
    (void *)Vram_LoadTexUpper, (void *)Vram_Tex, (void *)Vram_TexUpper, (void *)Vram_AllocTexture,
    (void *)Vram_Width, (void *)Vram_Height, (void *)Vram_TexAddr, (void *)Vram_ClutAddr,
    (void *)0x00000000u, (void *)0x00000000u, (void *)IK3_dtor, (void *)IK3_Loaded,
    (void *)0x00000000u, (void *)0x00000000u, (void *)IK2_dtor, (void *)IK2_Loaded,
    (void *)0x00000000u, (void *)0x00000000u, (void *)CharModel_dtor, (void *)CharModel_Loaded,
    (void *)CharModel_Frame, (void *)HumanModel_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)Model_Vt2C,
    (void *)Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)HumanModel_Vt3C,
    (void *)HumanModel_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)HumanModel_PlantFeet,
    (void *)Model_FeetReplace, (void *)HumanModel_FeetUp, (void *)HumanModel_Vt58, (void *)Model_EaseTilt,
    (void *)HumanModel_HeadPos, (void *)Model_FootDown, (void *)CharModel_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)HumanModel_Vt74, (void *)HumanModel_Vt78, (void *)HumanModel_Vt7C,
    (void *)HumanModel_HeadBone, (void *)Model_Part0, (void *)Model_Part1, (void *)Model_Part2,
    (void *)Model_Part3, (void *)Model_Vt94, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)Model_ModelFile, (void *)CharModel_MarkerFile, (void *)Model_Textures, (void *)Model_VtAC,
    (void *)Model_VtB0, (void *)CharModel_SecondaryMotion, (void *)0x00000000u, (void *)CharModel_SetFlag,
    (void *)CharModel_SetPoint, (void *)CharModel_ShowParts, (void *)CharModel_SetVector, (void *)CharModel_VtCC,
    (void *)CharModel_VtD0, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Shadow_dtor, (void *)Shadow_Draw,
    (void *)0x00000000u, (void *)0x00000000u, (void *)TexCache_Layer, (void *)TexCache_Entry,
    (void *)TexCache_AddGroup, (void *)TexCache_RemoveGroup, (void *)TexCache_ForgetLayers, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ModelBase_dtor, (void *)ModelBase_Loaded,
    (void *)Model_Release, (void *)Model_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)DogModel_dtor, (void *)DogModel_Loaded,
    (void *)DogModel_Frame, (void *)DogModel_AdjustBone, (void *)DogModel_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)DogModel_SetMatrix, (void *)Model_Vt2C,
    (void *)Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)Model_Vt3C,
    (void *)DogModel_BodyFrames, (void *)Model_SlopeKeep, (void *)DogModel_FloorLevel, (void *)DogModel_PlantFeet,
    (void *)DogModel_FeetReplace, (void *)DogModel_FeetUp, (void *)Model_Vt58, (void *)Model_EaseTilt,
    (void *)DogModel_HeadPos, (void *)DogModel_FootDown, (void *)Model_ClearDraw, (void *)DogModel_HalfHip,
    (void *)Model_Vt70, (void *)Model_Vt74, (void *)Model_Vt78, (void *)Model_Vt7C,
    (void *)DogModel_HeadBone, (void *)DogModel_Part0, (void *)DogModel_Part1, (void *)DogModel_Part2,
    (void *)DogModel_Part3, (void *)Model_Vt94, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)DogModel_ModelFile, (void *)DogModel_MarkerFile, (void *)DogModel_Textures, (void *)Model_VtAC,
    (void *)Model_VtB0, (void *)DogModel_SecondaryMotion, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Lights_dtor, (void *)Lights_TakeRoom,
    (void *)Lights_ForModel, (void *)Lights_ShadowLights, (void *)Lights_AreaTex0, (void *)Lights_GetLight,
    (void *)Lights_SetLight, (void *)Lights_RestoreLight, (void *)Lights_Get14, (void *)Lights_ShadowMayFall,
    (void *)Lights_ShadowQuad, (void *)Lights_Frame, (void *)Lights_AddDoorway, (void *)Lights_Blockers,
    (void *)Lights_BlockerCount, (void *)Lights_Set944, (void *)Lights_ExtraLight, (void *)Lights_ExtraLightI,
    (void *)0x00000000u, (void *)0x00000000u, (void *)LightsBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Events_dtor, (void *)EventCmd_Skip,
    (void *)EventCond_Eval, (void *)EventCond_Skip, (void *)0x00000000u, (void *)0x00000000u,
    (void *)func_0020C390, (void *)func_0020C380, (void *)func_0020C360, (void *)func_0020C370,
    (void *)func_0020C350, (void *)func_0020C340, (void *)func_0020C330, (void *)func_0020C320,
    (void *)func_0020C310, (void *)func_0020C300, (void *)func_0020C2F0, (void *)func_0020C2E0,
    (void *)func_0020C2D0, (void *)func_0020C2C0, (void *)func_0020C2B0, (void *)func_0020C2A0,
    (void *)func_0020C290, (void *)func_0020C280, (void *)func_0020C270, (void *)func_0020C260,
    (void *)func_0020C250, (void *)func_0020C240, (void *)func_0020C230, (void *)func_0020C220,
    (void *)func_0020C210, (void *)func_0020C200, (void *)func_0020C1F0, (void *)func_0020C1E0,
    (void *)func_0020C1D0, (void *)Events_SetScript, (void *)Events_CharEnter, (void *)Events_SetVar,
    (void *)Events_GetVar, (void *)Events_ScriptCharValue, (void *)Events_OpenMessage, (void *)Events_CloseMessage,
    (void *)Events_Fade, (void *)Events_SetFadeColour, (void *)Events_DrawFade, (void *)Events_ScenePlaying,
    (void *)Events_CueMoved, (void *)Events_CurrentBlock, (void *)Events_NearestPoint, (void *)Events_SetPoint,
    (void *)Events_ScriptRoom, (void *)Events_ShowText, (void *)Events_InArea, (void *)Events_CharInArea,
    (void *)Events_StartAction, (void *)Events_GiveScript, (void *)Events_AreaMiddle, (void *)Events_RoomPoint,
    (void *)Events_Get703, (void *)Events_TestBit, (void *)Events_SetBit, (void *)Events_ClearBit,
    (void *)Events_ThingPlace, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room101_dtor, (void *)Room101_EnterScript,
    (void *)Room101_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomBase_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room101_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room101_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room100_dtor, (void *)Room100_EnterScript,
    (void *)Room100_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomBase_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room100_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room100_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room82_dtor, (void *)Room82_EnterScript,
    (void *)Room82_Phase1Script, (void *)Room82_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room82_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room82_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room82_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room67_dtor, (void *)Room67_EnterScript,
    (void *)Room67_Phase1Script, (void *)Room67_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room67_Phase5Script, (void *)Room67_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room67_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room67_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room65_dtor, (void *)Room65_EnterScript,
    (void *)Room65_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomBase_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room65_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room65_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room64_dtor, (void *)Room64_EnterScript,
    (void *)Room64_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomBase_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room64_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room64_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room5F_dtor, (void *)Room5F_EnterScript,
    (void *)Room5F_Phase1Script, (void *)Room5F_Phase2Script, (void *)Room5F_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room5F_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room5F_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room5F_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room5E_dtor, (void *)Room5E_EnterScript,
    (void *)Room5E_Phase1Script, (void *)Room5E_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room5E_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room5E_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room5E_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room5B_dtor, (void *)Room5B_EnterScript,
    (void *)Room5B_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomBase_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room5B_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room5B_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room4D_dtor, (void *)Room4D_EnterScript,
    (void *)Room4D_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomBase_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room4D_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room4D_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room44_dtor, (void *)Room44_EnterScript,
    (void *)Room44_Phase1Script, (void *)Room44_Phase2Script, (void *)Room44_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room44_Phase5Script, (void *)Room44_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room44_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room44_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room42_dtor, (void *)Room42_EnterScript,
    (void *)Room42_Phase1Script, (void *)Room42_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room42_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room42_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room42_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room41_dtor, (void *)Room41_EnterScript,
    (void *)Room41_Phase1Script, (void *)Room41_Phase2Script, (void *)Room41_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room41_Phase5Script, (void *)Room41_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room41_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room41_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room16_dtor, (void *)Room16_EnterScript,
    (void *)Room16_Phase1Script, (void *)Room16_Phase2Script, (void *)Room16_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room16_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room16_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room16_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room07_dtor, (void *)Room07_EnterScript,
    (void *)Room07_Phase1Script, (void *)Room07_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room07_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room07_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room07_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room05_dtor, (void *)Room05_EnterScript,
    (void *)Room05_Phase1Script, (void *)Room05_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room05_Phase5Script, (void *)Room05_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room05_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room01_dtor, (void *)Room01_EnterScript,
    (void *)Room01_Phase1Script, (void *)Room01_Phase2Script, (void *)Room01_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room01_Phase5Script, (void *)Room01_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room01_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room01_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)DogModelA_dtor, (void *)DogModel_Loaded,
    (void *)DogModel_Frame, (void *)DogModel_AdjustBone, (void *)DogModel_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)DogModel_SetMatrix, (void *)Model_Vt2C,
    (void *)Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)Model_Vt3C,
    (void *)DogModel_BodyFrames, (void *)Model_SlopeKeep, (void *)DogModel_FloorLevel, (void *)DogModel_PlantFeet,
    (void *)DogModel_FeetReplace, (void *)DogModel_FeetUp, (void *)Model_Vt58, (void *)Model_EaseTilt,
    (void *)DogModel_HeadPos, (void *)DogModel_FootDown, (void *)Model_ClearDraw, (void *)DogModel_HalfHip,
    (void *)Model_Vt70, (void *)Model_Vt74, (void *)Model_Vt78, (void *)Model_Vt7C,
    (void *)DogModel_HeadBone, (void *)DogModel_Part0, (void *)DogModel_Part1, (void *)DogModel_Part2,
    (void *)DogModel_Part3, (void *)Model_Vt94, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)DogModelA_KindTable, (void *)DogModel_MarkerFile, (void *)DogModelA_Table, (void *)Model_VtAC,
    (void *)Model_VtB0, (void *)DogModel_SecondaryMotion, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)DogModelB_dtor, (void *)DogModel_Loaded,
    (void *)DogModel_Frame, (void *)DogModel_AdjustBone, (void *)DogModel_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)DogModel_SetMatrix, (void *)Model_Vt2C,
    (void *)Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)Model_Vt3C,
    (void *)DogModel_BodyFrames, (void *)Model_SlopeKeep, (void *)DogModel_FloorLevel, (void *)DogModel_PlantFeet,
    (void *)DogModel_FeetReplace, (void *)DogModel_FeetUp, (void *)Model_Vt58, (void *)Model_EaseTilt,
    (void *)DogModel_HeadPos, (void *)DogModel_FootDown, (void *)Model_ClearDraw, (void *)DogModel_HalfHip,
    (void *)Model_Vt70, (void *)Model_Vt74, (void *)Model_Vt78, (void *)Model_Vt7C,
    (void *)DogModel_HeadBone, (void *)DogModel_Part0, (void *)DogModel_Part1, (void *)DogModel_Part2,
    (void *)DogModel_Part3, (void *)Model_Vt94, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)DogModelB_KindTable, (void *)DogModel_MarkerFile, (void *)DogModelB_Table, (void *)Model_VtAC,
    (void *)Model_VtB0, (void *)DogModel_SecondaryMotion, (void *)0x00000000u, (void *)0x00000000u,
    (void *)EventPoint_dtor, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Obj46BA80_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)EventsBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Daniella_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Daniella_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Daniella_Update, (void *)Pursuer_LeaveScreen, (void *)Pursuer_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Pursuer_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Daniella_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)Daniella_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)Daniella_ExitArg, (void *)Daniella_Setup, (void *)Daniella_ModelFiles, (void *)Daniella_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Pursuer_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Daniella_AttackTable, (void *)Pursuer_PickAttack, (void *)Daniella_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Daniella_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Daniella_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Daniella_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Daniella_ThreatAmount, (void *)Daniella_AttackAnimA,
    (void *)Daniella_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Daniella_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)Daniella_Kind,
    (void *)0x00000000u, (void *)0x00000000u, (void *)GameBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)SceneTable_dtor, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SndDriver_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)SndDriver_dtor_thunk, (void *)SndDriver_StopAll_thunk, (void *)SndDriver_StopMasks_thunk,
    (void *)SndDriver_Play_thunk, (void *)SndDriver_StopSound_thunk, (void *)SndDriver_SeqPlay_thunk, (void *)SndDriver_SeqStop_thunk,
    (void *)SndDriver_SeqRestart_thunk, (void *)SndDriver_SeqParam1B_thunk, (void *)SndDriver_SeqParam2B_thunk, (void *)SndDriver_SeqParamPtr_thunk,
    (void *)SndDriver_SeqParam1_thunk, (void *)SndDriver_SeqParam2_thunk, (void *)SndDriver_SetSeqVolume_thunk, (void *)SndDriver_SeqVolume_thunk,
    (void *)SndDriver_SetSeqTempo_thunk, (void *)SndDriver_SeqTempo_thunk, (void *)SndDriver_Header_thunk, (void *)SndDriver_Table_thunk,
    (void *)SndDriver_Sequence_thunk, (void *)SndDriver_SamplesNow_thunk, (void *)SndDriver_QueueSamples_thunk, (void *)SndDriver_Register_thunk,
    (void *)SndDriver_Unload_thunk, (void *)SndDriver_SetOutputMode_thunk, (void *)SndDriver_OutputMode_thunk, (void *)SndDriver_TransferRunning_thunk,
    (void *)SndDriver_BankLoading_thunk, (void *)SndDriver_GetVolume_thunk, (void *)SndDriver_SetVolume_thunk, (void *)SndDriver_LoadPart_thunk,
    (void *)SndDriver_CancelFile_thunk, (void *)SndDriver_DropFile_thunk, (void *)SndDriver_DropFiles_thunk, (void *)SndDriver_BankFileLoaded_thunk,
    (void *)SndDriver_PlacedState_thunk, (void *)SndDriver_StopPlaced_thunk, (void *)SndDriver_StopPlacedButBanks_thunk, (void *)SndDriver_StopVoices_thunk,
    (void *)SndDriver_BankLoaded_thunk, (void *)SndDriver_SetSound_thunk, (void *)SndDriver_SetMaster_thunk, (void *)SndDriver_SendMaster_thunk,
    (void *)SndDriver_PlayPlaced_thunk, (void *)SndDriver_PlayPositioned_thunk, (void *)SndDriver_Block3D_thunk, (void *)SndDriver_StopAll,
    (void *)SndDriver_StopMasks, (void *)SndDriver_Play, (void *)SndDriver_PlayPlaced, (void *)SndDriver_StopSound,
    (void *)SndDriver_SeqPlay, (void *)SndDriver_SeqStop, (void *)SndDriver_SeqRestart, (void *)SndDriver_SeqParam1B,
    (void *)SndDriver_SeqParam2B, (void *)SndDriver_SeqParamPtr, (void *)SndDriver_SeqParam1, (void *)SndDriver_SeqParam2,
    (void *)SndDriver_SetSeqVolume, (void *)SndDriver_SeqVolume, (void *)SndDriver_SetSeqTempo, (void *)SndDriver_SeqTempo,
    (void *)SndDriver_Block3D, (void *)SndDriver_Header, (void *)SndDriver_Table, (void *)SndDriver_Sequence,
    (void *)SndDriver_SamplesNow, (void *)SndDriver_QueueSamples, (void *)SndDriver_Register, (void *)SndDriver_Unload,
    (void *)SndDriver_SetOutputMode, (void *)SndDriver_OutputMode, (void *)SndDriver_TransferRunning, (void *)SndDriver_BankLoading,
    (void *)SndDriver_GetVolume, (void *)SndDriver_SetVolume, (void *)SndDriver_LoadPart, (void *)SndDriver_CancelFile,
    (void *)SndDriver_DropFile, (void *)SndDriver_DropFiles, (void *)SndDriver_BankFileLoaded, (void *)SndDriver_PlacedState,
    (void *)SndDriver_StopPlaced, (void *)SndDriver_StopPlacedButBanks, (void *)SndDriver_StopVoices, (void *)SndDriver_PlayPositioned,
    (void *)SndDriver_SetSound, (void *)SndDriver_SetMaster, (void *)SndDriver_SendMaster, (void *)SndDriver_BankLoaded,
    (void *)0x00000000u, (void *)0x00000000u, (void *)DebilitasModel_dtor, (void *)DebilitasModel_Loaded,
    (void *)DebilitasModel_Frame, (void *)HumanModel_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)Model_Vt2C,
    (void *)Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)DebilitasModel_Vt3C,
    (void *)HumanModel_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)HumanModel_PlantFeet,
    (void *)Model_FeetReplace, (void *)HumanModel_FeetUp, (void *)HumanModel_Vt58, (void *)Model_EaseTilt,
    (void *)HumanModel_HeadPos, (void *)Model_FootDown, (void *)Model_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)HumanModel_Vt74, (void *)HumanModel_Vt78, (void *)HumanModel_Vt7C,
    (void *)HumanModel_HeadBone, (void *)DebilitasModel_Part0, (void *)DebilitasModel_Part1, (void *)DebilitasModel_Part2,
    (void *)DebilitasModel_Part3, (void *)Model_Vt94, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)Model_ModelFile, (void *)Model_MarkerFile, (void *)Model_Textures, (void *)Model_VtAC,
    (void *)Model_VtB0, (void *)DebilitasModel_SecondaryMotion, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)HumanModel_dtor, (void *)HumanModel_Loaded,
    (void *)HumanModel_Frame, (void *)HumanModel_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)Model_Vt2C,
    (void *)Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)HumanModel_Vt3C,
    (void *)HumanModel_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)HumanModel_PlantFeet,
    (void *)Model_FeetReplace, (void *)HumanModel_FeetUp, (void *)HumanModel_Vt58, (void *)Model_EaseTilt,
    (void *)HumanModel_HeadPos, (void *)Model_FootDown, (void *)Model_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)HumanModel_Vt74, (void *)HumanModel_Vt78, (void *)HumanModel_Vt7C,
    (void *)HumanModel_HeadBone, (void *)Model_Part0, (void *)Model_Part1, (void *)Model_Part2,
    (void *)Model_Part3, (void *)Model_Vt94, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)Model_ModelFile, (void *)Model_MarkerFile, (void *)Model_Textures, (void *)Model_VtAC,
    (void *)Model_VtB0, (void *)Model_SecondaryMotion, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)NPC_dtor, (void *)NPC_Reset,
    (void *)Actor_Cleanup, (void *)Actor_LoadFiles, (void *)Actor_FilesLoading, (void *)Actor_FilesLoaded,
    (void *)Actor_Unload, (void *)Character_RememberPos, (void *)Character_Place, (void *)Actor_LightChange,
    (void *)Actor_Update, (void *)Actor_LeaveScreen, (void *)Actor_ShowUp, (void *)Actor_CanAct,
    (void *)0x00000000u, (void *)Character_Think, (void *)Character_PlaceModel, (void *)Character_Disable,
    (void *)Character_Enable, (void *)Character_LoadMessage, (void *)Character_Deactivate, (void *)Character_Activate,
    (void *)Character_ResetBehaviour, (void *)0x00000000u, (void *)Character_EventConcerns, (void *)Character_SaveState,
    (void *)Character_LoadState, (void *)Character_AttackPoint, (void *)Character_LeftBehind, (void *)Character_BackToStance,
    (void *)Character_RegionFade, (void *)0x00000000u, (void *)Character_EventCommand, (void *)Character_BackToNormal,
    (void *)Character_EventReset, (void *)Character_TimerDown, (void *)Character_IsBusy, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)0x00000000u,
    (void *)0x00000000u, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Obstacles_dtor, (void *)Obstacles_LoadMotions,
    (void *)Obstacles_PlaceAt, (void *)Obstacles_PlaceSaved, (void *)Obstacles_Push, (void *)Obstacles_Pull,
    (void *)Obstacles_Settle, (void *)Obstacles_RingMask, (void *)Obstacles_StartMove, (void *)Obstacles_Moved,
    (void *)Obstacles_CanPush, (void *)Obstacles_IsSquare, (void *)Obstacles_Stop, (void *)Obstacles_SetTarget,
    (void *)Obstacles_SetSaved, (void *)Obstacles_TakeSaved, (void *)Obstacles_KeepSpot, (void *)Obstacles_ModelBack,
    (void *)Obstacles_Pos, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ObstaclesBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Rooms_dtor, (void *)Rooms_SetState,
    (void *)Rooms_ExitDoor, (void *)Rooms_OtherExit, (void *)Rooms_OtherRoom, (void *)Rooms_DoorLeadsTo,
    (void *)Rooms_ExitTriOut, (void *)Rooms_ExitTriIn, (void *)Rooms_ExitTriThrough, (void *)Rooms_ExitPointOut,
    (void *)Rooms_ExitPointIn, (void *)Rooms_ExitPointThrough, (void *)Rooms_DoorTri, (void *)Rooms_DoorExit,
    (void *)Rooms_ExitDoorFlags, (void *)Rooms_DoorFlags, (void *)Rooms_CameraArea, (void *)Rooms_ExitHasSideFlag,
    (void *)Rooms_ExitSideFlag, (void *)Rooms_DoorFromSide, (void *)Rooms_ExitFromOther, (void *)Rooms_DoorFromOther,
    (void *)Rooms_DoorClosedOff, (void *)Rooms_CloseOff, (void *)Rooms_Reopen, (void *)Rooms_DoorSideRoom,
    (void *)Rooms_ExitLoops, (void *)Rooms_ExitLeads, (void *)Rooms_ExitNotLocked, (void *)Rooms_SavedState,
    (void *)Rooms_Centre, (void *)Rooms_Distance, (void *)Rooms_NeighbourCount, (void *)Rooms_ExitHeading,
    (void *)Rooms_Rebuild, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomsBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoutePlanner_dtor, (void *)RoutePlanner_FindRoute,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoutePlannerBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Doors_dtor, (void *)Doors_StartAnim,
    (void *)Doors_OnSide, (void *)Doors_AnimUserSpot, (void *)Doors_Side, (void *)Doors_ShutSide,
    (void *)Doors_OpenSide, (void *)Doors_TakeSection8, (void *)Doors_OpenFromNear, (void *)Doors_AtDoor,
    (void *)Doors_IsIdle, (void *)Doors_GetPos, (void *)Doors_GetPoint20, (void *)Doors_GetRestTurn,
    (void *)Doors_HasExit, (void *)Doors_GetNavTri, (void *)Doors_DataBuffer, (void *)Doors_SelectBuffer,
    (void *)Doors_SpotJustBehind, (void *)Doors_SpotAhead, (void *)Doors_SpotBehind, (void *)Doors_SaveAll,
    (void *)Doors_SaveDoor, (void *)Doors_GetOpenAngle, (void *)Doors_SetOpened, (void *)Doors_InArea,
    (void *)Doors_IsOpened, (void *)Doors_TurnBy, (void *)Doors_TurnTo, (void *)Doors_RoomIn,
    (void *)Doors_Refresh, (void *)Doors_SetBits, (void *)Doors_SetFlag82, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)DoorsBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)func_00225760, (void *)func_00225750, (void *)func_00225740, (void *)func_00225730,
    (void *)func_00225720, (void *)func_00225710, (void *)func_00225700, (void *)func_002256F0,
    (void *)func_002256E0, (void *)func_002256D0, (void *)func_002256C0, (void *)func_002256B0,
    (void *)func_002256A0, (void *)func_00225690, (void *)func_00225680, (void *)CamDirector_dtor,
    (void *)CamDirector_Restart, (void *)CamDirector_Follow, (void *)CamDirector_PathSeek, (void *)CamDirector_PathLength,
    (void *)CamDirector_SetupChanged, (void *)CamDirector_SetSetup, (void *)CamDirector_Setup, (void *)CamDirector_FreeMode,
    (void *)CamDirector_NextSetup, (void *)CamDirector_Set50, (void *)CamDirector_ModeDefault, (void *)CamDirector_ModeEvent,
    (void *)CamDirector_HoldEffect1C, (void *)EventCam_Set, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)CamDirectorBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MovieLib_dtor, (void *)MovieLib_Setup,
    (void *)MovieLib_SetWork, (void *)MovieLib_Shutdown_14, (void *)MovieLib_CreatePlayer, (void *)MovieLib_MovieSize,
    (void *)MovieLib_WorkSize, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomMeshes_dtor, (void *)RoomMesh_Draw,
    (void *)0x00000000u, (void *)0x00000000u, (void *)PlacedMesh_dtor, (void *)PlacedMesh_Draw,
    (void *)0x00000000u, (void *)0x00000000u, (void *)PoolEntry_dtor, (void *)PoolEntry_Id,
    (void *)PoolEntry_Kind, (void *)PoolEntry_Actions, (void *)PoolEntry_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)PoolEntry_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item12_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item12_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item12_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item13_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item13_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item13_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item3E_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)PoolEntry_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item3E_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)PlainItem_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)PoolEntry_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemClassC920_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassC920_Kind, (void *)ItemClassC920_Actions, (void *)ItemClassC920_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)PoolEntry_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item70_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassCB50_Kind, (void *)ItemClassCB50_Actions, (void *)ItemClassCB50_IsCounted, (void *)Item70_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item70_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemClassCB50_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item71_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassCB50_Kind, (void *)ItemClassCB50_Actions, (void *)ItemClassCB50_IsCounted, (void *)Item71_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item71_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemClassCB50_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item72_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassCB50_Kind, (void *)ItemClassCB50_Actions, (void *)ItemClassCB50_IsCounted, (void *)Item72_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item72_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemClassCB50_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item73_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassCB50_Kind, (void *)ItemClassCB50_Actions, (void *)ItemClassCB50_IsCounted, (void *)Item73_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item73_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemClassCB50_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item74_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassCB50_Kind, (void *)ItemClassCB50_Actions, (void *)ItemClassCB50_IsCounted, (void *)Item74_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item74_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemClassCB50_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item75_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassCB50_Kind, (void *)ItemClassCB50_Actions, (void *)ItemClassCB50_IsCounted, (void *)Item75_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item75_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemClassCB50_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemClassCB50_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassCB50_Kind, (void *)ItemClassCB50_Actions, (void *)ItemClassCB50_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)PoolEntry_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemClassCB50_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item80_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassCCE0_Kind, (void *)ItemClassCCE0_Actions, (void *)ItemClassCCE0_IsCounted, (void *)Item80_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item80_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item81_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassCCE0_Kind, (void *)ItemClassCCE0_Actions, (void *)ItemClassCCE0_IsCounted, (void *)Item81_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item81_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item82_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassCCE0_Kind, (void *)ItemClassCCE0_Actions, (void *)ItemClassCCE0_IsCounted, (void *)Item82_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item82_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item83_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassCCE0_Kind, (void *)ItemClassCCE0_Actions, (void *)ItemClassCCE0_IsCounted, (void *)Item83_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item83_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemClassCCE0_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassCCE0_Kind, (void *)ItemClassCCE0_Actions, (void *)ItemClassCCE0_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)PoolEntry_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item86_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassCE70_Kind, (void *)ItemClassCE70_Actions, (void *)ItemClassCE70_IsCounted, (void *)Item86_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item86_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item87_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassCE70_Kind, (void *)ItemClassCE70_Actions, (void *)ItemClassCE70_IsCounted, (void *)Item87_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item87_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item88_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassCE70_Kind, (void *)ItemClassCE70_Actions, (void *)ItemClassCE70_IsCounted, (void *)Item88_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item88_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item89_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassCE70_Kind, (void *)ItemClassCE70_Actions, (void *)ItemClassCE70_IsCounted, (void *)Item89_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item89_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemClassCE70_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassCE70_Kind, (void *)ItemClassCE70_Actions, (void *)ItemClassCE70_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)PoolEntry_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item8A_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD000_Kind, (void *)ItemClassD000_Actions, (void *)ItemClassD000_IsCounted, (void *)Item8A_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item8A_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item8B_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD000_Kind, (void *)ItemClassD000_Actions, (void *)ItemClassD000_IsCounted, (void *)Item8B_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item8B_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item8C_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD000_Kind, (void *)ItemClassD000_Actions, (void *)ItemClassD000_IsCounted, (void *)Item8C_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item8C_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item8D_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD000_Kind, (void *)ItemClassD000_Actions, (void *)ItemClassD000_IsCounted, (void *)Item8D_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item8D_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemClassD000_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD000_Kind, (void *)ItemClassD000_Actions, (void *)ItemClassD000_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)PoolEntry_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item90_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD320_Kind, (void *)ItemClassD320_Actions, (void *)ItemClassD320_IsCounted, (void *)Item90_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item90_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item91_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD320_Kind, (void *)ItemClassD320_Actions, (void *)ItemClassD320_IsCounted, (void *)Item91_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item91_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item92_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD320_Kind, (void *)ItemClassD320_Actions, (void *)ItemClassD320_IsCounted, (void *)Item92_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item92_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item93_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD320_Kind, (void *)ItemClassD320_Actions, (void *)ItemClassD320_IsCounted, (void *)Item93_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item93_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item94_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD320_Kind, (void *)ItemClassD320_Actions, (void *)ItemClassD320_IsCounted, (void *)Item94_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item94_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item95_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD320_Kind, (void *)ItemClassD320_Actions, (void *)ItemClassD320_IsCounted, (void *)Item95_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item95_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item97_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD320_Kind, (void *)ItemClassD320_Actions, (void *)ItemClassD320_IsCounted, (void *)Item97_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item97_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item98_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD320_Kind, (void *)ItemClassD320_Actions, (void *)ItemClassD320_IsCounted, (void *)Item98_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item98_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item9B_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD320_Kind, (void *)ItemClassD320_Actions, (void *)ItemClassD320_IsCounted, (void *)Item9B_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item9B_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemClassD320_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD320_Kind, (void *)ItemClassD320_Actions, (void *)ItemClassD320_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)PoolEntry_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemA4_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD640_Kind, (void *)ItemClassD640_Actions, (void *)ItemClassD640_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)ItemA4_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemA4_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemA5_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD640_Kind, (void *)ItemClassD640_Actions, (void *)ItemClassD640_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)ItemA5_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemA5_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemA6_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD640_Kind, (void *)ItemClassD640_Actions, (void *)ItemClassD640_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)ItemA6_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemA6_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemA7_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD640_Kind, (void *)ItemClassD640_Actions, (void *)ItemClassD640_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)ItemA7_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemA7_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemA8_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD640_Kind, (void *)ItemClassD640_Actions, (void *)ItemClassD640_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)ItemA8_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemA8_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemA9_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD640_Kind, (void *)ItemClassD640_Actions, (void *)ItemClassD640_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)ItemA9_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemA9_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemAA_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD640_Kind, (void *)ItemClassD640_Actions, (void *)ItemClassD640_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)ItemAA_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemAA_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemAB_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD640_Kind, (void *)ItemClassD640_Actions, (void *)ItemClassD640_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)ItemAB_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemAB_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemAC_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD640_Kind, (void *)ItemClassD640_Actions, (void *)ItemClassD640_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)ItemAC_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemAC_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemClassD640_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD640_Kind, (void *)ItemClassD640_Actions, (void *)ItemClassD640_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)PoolEntry_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item40_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassC920_Kind, (void *)ItemClassC920_Actions, (void *)ItemClassC920_IsCounted, (void *)Item40_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item40_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item40_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item41_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassC920_Kind, (void *)ItemClassC920_Actions, (void *)ItemClassC920_IsCounted, (void *)Item41_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item41_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item41_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomEffectBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomEffectBase_Noop18, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)GameOverBase_dtor, (void *)Tint_Start,
    (void *)Tint_Update, (void *)Tint_Draw, (void *)Tint_SetParams, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)PlacedModelBase_dtor, (void *)PlacedModel_Draw,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Obj46D780_dtor, (void *)func_00268180,
    (void *)0x00000000u, (void *)0x00000000u, (void *)TintDrawer_dtor, (void *)func_002685F0,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Bloom_dtor, (void *)Bloom_Draw,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ScreenBlend_dtor, (void *)ScreenBlend_Start,
    (void *)ScreenBlend_Update, (void *)ScreenBlend_Draw, (void *)ScreenBlend_SetParams, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Message_SetSlot, (void *)Message_ReleaseSlot,
    (void *)Message_Show, (void *)Message_ShowNothing, (void *)Message_Unhide, (void *)Message_Hide,
    (void *)Message_ForgetLast, (void *)Message_DrawTarget, (void *)Message_TexEntry, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)DoorShadow_dtor, (void *)DoorShadow_Draw,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Pursuer_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Pursuer_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Pursuer_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)Pursuer_Setup, (void *)Pursuer_ModelFiles, (void *)Pursuer_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Pursuer_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)NavGroups_dtor, (void *)NavGroups_SetGroup,
    (void *)NavGroups_ClearGroup, (void *)NavGroups_Holds, (void *)NavGroups_SetTri, (void *)NavGroups_ClearTri,
    (void *)0x00000000u, (void *)0x00000000u, (void *)NavGroupsBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomBase_dtor, (void *)RoomBase_EnterScript,
    (void *)RoomBase_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomBase_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)RoomBase_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room00_dtor, (void *)Room00_EnterScript,
    (void *)Room00_Phase1Script, (void *)Room00_Phase2Script, (void *)Room00_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room00_ActionScript, (void *)Room00_Command, (void *)Room00_Condition,
    (void *)Room00_CharEnterScript, (void *)Room00_ObjectName, (void *)Room00_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room02_dtor, (void *)Room02_EnterScript,
    (void *)Room02_Phase1Script, (void *)Room02_Phase2Script, (void *)Room02_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room02_Phase5Script, (void *)Room02_ActionScript, (void *)Room02_Command, (void *)Room02_Condition,
    (void *)Room02_CharEnterScript, (void *)Room02_ObjectName, (void *)Room02_Table38, (void *)Room02_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room03_dtor, (void *)Room03_EnterScript,
    (void *)Room03_Phase1Script, (void *)Room03_Phase2Script, (void *)Room03_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room03_Phase5Script, (void *)Room03_ActionScript, (void *)Room03_Command, (void *)RoomBase_Condition,
    (void *)Room03_CharEnterScript, (void *)Room03_ObjectName, (void *)Room03_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room04_dtor, (void *)Room04_EnterScript,
    (void *)Room04_Phase1Script, (void *)Room04_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room04_ActionScript, (void *)Room04_Command, (void *)RoomBase_Condition,
    (void *)Room04_CharEnterScript, (void *)Room04_ObjectName, (void *)Room04_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room06_dtor, (void *)Room06_EnterScript,
    (void *)Room06_Phase1Script, (void *)Room06_Phase2Script, (void *)Room06_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room06_Phase5Script, (void *)Room06_ActionScript, (void *)Room06_Command, (void *)RoomBase_Condition,
    (void *)Room06_CharEnterScript, (void *)Room06_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room08_dtor, (void *)Room08_EnterScript,
    (void *)Room08_Phase1Script, (void *)Room08_Phase2Script, (void *)Room08_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room08_Phase5Script, (void *)Room08_ActionScript, (void *)Room08_Command, (void *)Room08_Condition,
    (void *)Room08_CharEnterScript, (void *)Room08_ObjectName, (void *)Room08_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room09_dtor, (void *)Room09_EnterScript,
    (void *)Room09_Phase1Script, (void *)Room09_Phase2Script, (void *)Room09_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room09_Phase5Script, (void *)Room09_ActionScript, (void *)RoomBase_Command, (void *)Room09_Condition,
    (void *)Room09_CharEnterScript, (void *)Room09_ObjectName, (void *)Room09_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room0A_dtor, (void *)Room0A_EnterScript,
    (void *)Room0A_Phase1Script, (void *)Room0A_Phase2Script, (void *)Room0A_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room0A_ActionScript, (void *)Room0A_Command, (void *)RoomBase_Condition,
    (void *)Room0A_CharEnterScript, (void *)Room0A_ObjectName, (void *)Room0A_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room0B_dtor, (void *)Room0B_EnterScript,
    (void *)Room0B_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room0B_ActionScript, (void *)RoomBase_Command, (void *)Room0B_Condition,
    (void *)Room0B_CharEnterScript, (void *)Room0B_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room0C_dtor, (void *)Room0C_EnterScript,
    (void *)Room0C_Phase1Script, (void *)Room0C_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room0C_ActionScript, (void *)Room0C_Command, (void *)RoomBase_Condition,
    (void *)Room0C_CharEnterScript, (void *)Room0C_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room0D_dtor, (void *)Room0D_EnterScript,
    (void *)Room0D_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room0D_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room0D_CharEnterScript, (void *)Room0D_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room0E_dtor, (void *)Room0E_EnterScript,
    (void *)Room0E_Phase1Script, (void *)Room0E_Phase2Script, (void *)Room0E_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room0E_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room0E_CharEnterScript, (void *)Room0E_ObjectName, (void *)Room0E_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room0F_dtor, (void *)Room0F_EnterScript,
    (void *)Room0F_Phase1Script, (void *)Room0F_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room0F_ActionScript, (void *)Room0F_Command, (void *)RoomBase_Condition,
    (void *)Room0F_CharEnterScript, (void *)Room0F_ObjectName, (void *)Room0F_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room10_dtor, (void *)Room10_EnterScript,
    (void *)Room10_Phase1Script, (void *)Room10_Phase2Script, (void *)Room10_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room10_Phase5Script, (void *)Room10_ActionScript, (void *)RoomBase_Command, (void *)Room10_Condition,
    (void *)Room10_CharEnterScript, (void *)Room10_ObjectName, (void *)Room10_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room11_dtor, (void *)Room11_EnterScript,
    (void *)Room11_Phase1Script, (void *)Room11_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room11_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room11_CharEnterScript, (void *)Room11_ObjectName, (void *)Room11_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room12_dtor, (void *)Room12_EnterScript,
    (void *)Room12_Phase1Script, (void *)Room12_Phase2Script, (void *)Room12_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room12_Phase5Script, (void *)Room12_ActionScript, (void *)Room12_Command, (void *)Room12_Condition,
    (void *)Room12_CharEnterScript, (void *)Room12_ObjectName, (void *)Room12_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room13_dtor, (void *)Room13_EnterScript,
    (void *)Room13_Phase1Script, (void *)Room13_Phase2Script, (void *)Room13_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room13_ActionScript, (void *)Room13_Command, (void *)RoomBase_Condition,
    (void *)Room13_CharEnterScript, (void *)Room13_ObjectName, (void *)Room13_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room14_dtor, (void *)Room14_EnterScript,
    (void *)Room14_Phase1Script, (void *)Room14_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room14_ActionScript, (void *)Room14_Command, (void *)RoomBase_Condition,
    (void *)Room14_CharEnterScript, (void *)Room14_ObjectName, (void *)Room14_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room15_dtor, (void *)Room15_EnterScript,
    (void *)Room15_Phase1Script, (void *)Room15_Phase2Script, (void *)Room15_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room15_ActionScript, (void *)Room15_Command, (void *)Room15_Condition,
    (void *)Room15_CharEnterScript, (void *)Room15_ObjectName, (void *)Room15_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room18_dtor, (void *)Room18_EnterScript,
    (void *)Room18_Phase1Script, (void *)Room18_Phase2Script, (void *)Room18_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room18_Phase5Script, (void *)Room18_ActionScript, (void *)Room18_Command, (void *)RoomBase_Condition,
    (void *)Room18_CharEnterScript, (void *)Room18_ObjectName, (void *)Room18_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room19_dtor, (void *)Room19_EnterScript,
    (void *)Room19_Phase1Script, (void *)Room19_Phase2Script, (void *)Room19_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room19_ActionScript, (void *)RoomBase_Command, (void *)Room19_Condition,
    (void *)Room19_CharEnterScript, (void *)Room19_ObjectName, (void *)Room19_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room1A_dtor, (void *)Room1A_EnterScript,
    (void *)Room1A_Phase1Script, (void *)Room1A_Phase2Script, (void *)Room1A_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room1A_ActionScript, (void *)Room1A_Command, (void *)RoomBase_Condition,
    (void *)Room1A_CharEnterScript, (void *)Room1A_ObjectName, (void *)Room1A_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room1B_dtor, (void *)Room1B_EnterScript,
    (void *)Room1B_Phase1Script, (void *)Room1B_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room1B_ActionScript, (void *)Room1B_Command, (void *)RoomBase_Condition,
    (void *)Room1B_CharEnterScript, (void *)Room1B_ObjectName, (void *)Room1B_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room1C_dtor, (void *)Room1C_EnterScript,
    (void *)Room1C_Phase1Script, (void *)Room1C_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room1C_ActionScript, (void *)Room1C_Command, (void *)RoomBase_Condition,
    (void *)Room1C_CharEnterScript, (void *)Room1C_ObjectName, (void *)Room1C_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room1D_dtor, (void *)Room1D_EnterScript,
    (void *)Room1D_Phase1Script, (void *)Room1D_Phase2Script, (void *)Room1D_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room1D_Phase5Script, (void *)Room1D_ActionScript, (void *)Room1D_Command, (void *)Room1D_Condition,
    (void *)Room1D_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room1D_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room1E_dtor, (void *)Room1E_EnterScript,
    (void *)Room1E_Phase1Script, (void *)Room1E_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room1E_Phase5Script, (void *)Room1E_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room1E_CharEnterScript, (void *)Room1E_ObjectName, (void *)Room1E_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room1F_dtor, (void *)Room1F_EnterScript,
    (void *)Room1F_Phase1Script, (void *)Room1F_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room1F_ActionScript, (void *)Room1F_Command, (void *)RoomBase_Condition,
    (void *)Room1F_CharEnterScript, (void *)Room1F_ObjectName, (void *)Room1F_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room20_dtor, (void *)Room20_EnterScript,
    (void *)Room20_Phase1Script, (void *)Room20_Phase2Script, (void *)Room20_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room20_Phase5Script, (void *)Room20_ActionScript, (void *)Room20_Command, (void *)Room20_Condition,
    (void *)Room20_CharEnterScript, (void *)Room20_ObjectName, (void *)Room20_Table38, (void *)Room20_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room21_dtor, (void *)Room21_EnterScript,
    (void *)Room21_Phase1Script, (void *)Room21_Phase2Script, (void *)Room21_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room21_ActionScript, (void *)Room21_Command, (void *)Room21_Condition,
    (void *)Room21_CharEnterScript, (void *)Room21_ObjectName, (void *)Room21_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room22_dtor, (void *)Room22_EnterScript,
    (void *)Room22_Phase1Script, (void *)Room22_Phase2Script, (void *)Room22_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room22_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room22_CharEnterScript, (void *)Room22_ObjectName, (void *)Room22_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room23_dtor, (void *)Room23_EnterScript,
    (void *)Room23_Phase1Script, (void *)Room23_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room23_ActionScript, (void *)Room23_Command, (void *)Room23_Condition,
    (void *)Room23_CharEnterScript, (void *)Room23_ObjectName, (void *)Room23_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room24_dtor, (void *)Room24_EnterScript,
    (void *)Room24_Phase1Script, (void *)Room24_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room24_Phase5Script, (void *)Room24_ActionScript, (void *)Room24_Command, (void *)RoomBase_Condition,
    (void *)Room24_CharEnterScript, (void *)Room24_ObjectName, (void *)Room24_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room25_dtor, (void *)Room25_EnterScript,
    (void *)Room25_Phase1Script, (void *)Room25_Phase2Script, (void *)Room25_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room25_Phase5Script, (void *)Room25_ActionScript, (void *)Room25_Command, (void *)Room25_Condition,
    (void *)Room25_CharEnterScript, (void *)Room25_ObjectName, (void *)Room25_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room26_dtor, (void *)Room26_EnterScript,
    (void *)Room26_Phase1Script, (void *)Room26_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room26_ActionScript, (void *)Room26_Command, (void *)Room26_Condition,
    (void *)Room26_CharEnterScript, (void *)Room26_ObjectName, (void *)Room26_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room28_dtor, (void *)Room28_EnterScript,
    (void *)Room28_Phase1Script, (void *)Room28_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room28_Phase5Script, (void *)Room28_ActionScript, (void *)RoomBase_Command, (void *)Room28_Condition,
    (void *)Room28_CharEnterScript, (void *)Room28_ObjectName, (void *)Room28_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room29_dtor, (void *)Room29_EnterScript,
    (void *)Room29_Phase1Script, (void *)Room29_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room29_Phase5Script, (void *)Room29_ActionScript, (void *)RoomBase_Command, (void *)Room29_Condition,
    (void *)Room29_CharEnterScript, (void *)Room29_ObjectName, (void *)Room29_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room2A_dtor, (void *)Room2A_EnterScript,
    (void *)Room2A_Phase1Script, (void *)Room2A_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room2A_Phase5Script, (void *)Room2A_ActionScript, (void *)Room2A_Command, (void *)RoomBase_Condition,
    (void *)Room2A_CharEnterScript, (void *)Room2A_ObjectName, (void *)Room2A_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room2B_dtor, (void *)Room2B_EnterScript,
    (void *)Room2B_Phase1Script, (void *)Room2B_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room2B_ActionScript, (void *)Room2B_Command, (void *)RoomBase_Condition,
    (void *)Room2B_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room2B_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room2D_dtor, (void *)Room2D_EnterScript,
    (void *)Room2D_Phase1Script, (void *)Room2D_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room2D_ActionScript, (void *)Room2D_Command, (void *)Room2D_Condition,
    (void *)Room2D_CharEnterScript, (void *)Room2D_ObjectName, (void *)Room2D_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room40_dtor, (void *)Room40_EnterScript,
    (void *)Room40_Phase1Script, (void *)Room40_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room40_ActionScript, (void *)Room40_Command, (void *)RoomBase_Condition,
    (void *)Room40_CharEnterScript, (void *)Room40_ObjectName, (void *)Room40_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room43_dtor, (void *)Room43_EnterScript,
    (void *)Room43_Phase1Script, (void *)Room43_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room43_Phase5Script, (void *)Room43_ActionScript, (void *)Room43_Command, (void *)RoomBase_Condition,
    (void *)Room43_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room43_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room45_dtor, (void *)Room45_EnterScript,
    (void *)Room45_Phase1Script, (void *)Room45_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room45_Phase5Script, (void *)Room45_ActionScript, (void *)Room45_Command, (void *)RoomBase_Condition,
    (void *)Room45_CharEnterScript, (void *)Room45_ObjectName, (void *)Room45_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room49_dtor, (void *)Room49_EnterScript,
    (void *)Room49_Phase1Script, (void *)Room49_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room49_ActionScript, (void *)Room49_Command, (void *)RoomBase_Condition,
    (void *)Room49_CharEnterScript, (void *)Room49_ObjectName, (void *)Room49_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room4A_dtor, (void *)Room4A_EnterScript,
    (void *)Room4A_Phase1Script, (void *)Room4A_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room4A_Phase5Script, (void *)Room4A_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room4A_CharEnterScript, (void *)Room4A_ObjectName, (void *)Room4A_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room4B_dtor, (void *)Room4B_EnterScript,
    (void *)Room4B_Phase1Script, (void *)Room4B_Phase2Script, (void *)Room4B_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room4B_Phase5Script, (void *)Room4B_ActionScript, (void *)Room4B_Command, (void *)Room4B_Condition,
    (void *)Room4B_CharEnterScript, (void *)Room4B_ObjectName, (void *)Room4B_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room4C_dtor, (void *)Room4C_EnterScript,
    (void *)Room4C_Phase1Script, (void *)Room4C_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room4C_ActionScript, (void *)Room4C_Command, (void *)RoomBase_Condition,
    (void *)Room4C_CharEnterScript, (void *)Room4C_ObjectName, (void *)Room4C_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room4E_dtor, (void *)Room4E_EnterScript,
    (void *)Room4E_Phase1Script, (void *)Room4E_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room4E_Phase5Script, (void *)Room4E_ActionScript, (void *)Room4E_Command, (void *)RoomBase_Condition,
    (void *)Room4E_CharEnterScript, (void *)Room4E_ObjectName, (void *)Room4E_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room4F_dtor, (void *)Room4F_EnterScript,
    (void *)Room4F_Phase1Script, (void *)Room4F_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room4F_ActionScript, (void *)Room4F_Command, (void *)Room4F_Condition,
    (void *)Room4F_CharEnterScript, (void *)Room4F_ObjectName, (void *)Room4F_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room50_dtor, (void *)Room50_EnterScript,
    (void *)Room50_Phase1Script, (void *)Room50_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room50_Phase5Script, (void *)Room50_ActionScript, (void *)Room50_Command, (void *)RoomBase_Condition,
    (void *)Room50_CharEnterScript, (void *)Room50_ObjectName, (void *)Room50_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room51_dtor, (void *)Room51_EnterScript,
    (void *)Room51_Phase1Script, (void *)Room51_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room51_Phase5Script, (void *)Room51_ActionScript, (void *)Room51_Command, (void *)RoomBase_Condition,
    (void *)Room51_CharEnterScript, (void *)Room51_ObjectName, (void *)Room51_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room52_dtor, (void *)Room52_EnterScript,
    (void *)Room52_Phase1Script, (void *)Room52_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room52_ActionScript, (void *)Room52_Command, (void *)Room52_Condition,
    (void *)Room52_CharEnterScript, (void *)Room52_ObjectName, (void *)Room52_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room56_dtor, (void *)Room56_EnterScript,
    (void *)Room56_Phase1Script, (void *)Room56_Phase2Script, (void *)Room56_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room56_ActionScript, (void *)Room56_Command, (void *)RoomBase_Condition,
    (void *)Room56_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room56_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room57_dtor, (void *)Room57_EnterScript,
    (void *)Room57_Phase1Script, (void *)Room57_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room57_Phase5Script, (void *)Room57_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room57_CharEnterScript, (void *)Room57_ObjectName, (void *)Room57_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room58_dtor, (void *)Room58_EnterScript,
    (void *)Room58_Phase1Script, (void *)Room58_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room58_Phase5Script, (void *)Room58_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room58_CharEnterScript, (void *)Room58_ObjectName, (void *)Room58_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room59_dtor, (void *)Room59_EnterScript,
    (void *)Room59_Phase1Script, (void *)Room59_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room59_ActionScript, (void *)Room59_Command, (void *)RoomBase_Condition,
    (void *)Room59_CharEnterScript, (void *)Room59_ObjectName, (void *)Room59_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room5A_dtor, (void *)Room5A_EnterScript,
    (void *)Room5A_Phase1Script, (void *)Room5A_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room5A_Phase5Script, (void *)Room5A_ActionScript, (void *)Room5A_Command, (void *)RoomBase_Condition,
    (void *)Room5A_CharEnterScript, (void *)Room5A_ObjectName, (void *)Room5A_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room5C_dtor, (void *)Room5C_EnterScript,
    (void *)Room5C_Phase1Script, (void *)Room5C_Phase2Script, (void *)Room5C_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room5C_ActionScript, (void *)Room5C_Command, (void *)Room5C_Condition,
    (void *)Room5C_CharEnterScript, (void *)Room5C_ObjectName, (void *)Room5C_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room5D_dtor, (void *)Room5D_EnterScript,
    (void *)Room5D_Phase1Script, (void *)Room5D_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room5D_ActionScript, (void *)Room5D_Command, (void *)RoomBase_Condition,
    (void *)Room5D_CharEnterScript, (void *)Room5D_ObjectName, (void *)Room5D_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)TvScreenA_dtor, (void *)TvScreenA_Start,
    (void *)TvScreenA_Update, (void *)TvScreenA_Draw, (void *)TvScreenA_SetParams, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Movie_dtor, (void *)Scene_Update,
    (void *)Movie_Entry, (void *)Movie_Finish, (void *)Movie_Shot, (void *)Movie_Frame,
    (void *)Movie_TakeFrame, (void *)Movie_Draw, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Butterflies_dtor, (void *)Butterflies_Start,
    (void *)Butterflies_Update, (void *)Butterflies_Draw, (void *)Butterflies_SetParams, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MovieBlended_dtor, (void *)Scene_Update,
    (void *)MovieBlended_Entry, (void *)Movie_Finish, (void *)MovieBlended_Shot, (void *)Movie_Frame,
    (void *)MovieBlended_TakeFrame, (void *)MovieBlended_Draw, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MovieOpaque_dtor, (void *)Scene_Update,
    (void *)MovieOpaque_Entry, (void *)Movie_Finish, (void *)MovieOpaque_Shot, (void *)Movie_Frame,
    (void *)MovieOpaque_TakeFrame, (void *)MovieOpaque_Draw, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MovieHalf_dtor, (void *)Scene_Update,
    (void *)MovieHalf_Entry, (void *)Movie_Finish, (void *)Movie_Shot, (void *)Movie_Frame,
    (void *)MovieHalf_TakeFrame, (void *)MovieHalf_Draw, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Fog_dtor, (void *)Fog_Start,
    (void *)Fog_Update, (void *)Fog_Draw, (void *)Fog_SetParams, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)FogDrawer_dtor, (void *)func_002BB3E0,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MusicDir_dtor, (void *)MusicDir_Load,
    (void *)MusicDir_BanksIn, (void *)MusicDir_Reset, (void *)MusicDir_Release, (void *)MusicDir_Hold,
    (void *)MusicDir_SetMusicVolume, (void *)MusicDir_SetProgressVolume, (void *)MusicDir_Volume4, (void *)MusicDir_Get12,
    (void *)MusicDir_VolumeC, (void *)MusicDir_Get14, (void *)MusicDir_GlobalVolumeTo, (void *)MusicDir_Start,
    (void *)MusicDir_StageChans, (void *)MusicDir_SetVolume, (void *)MusicDir_FullVolume, (void *)MusicDir_Silence,
    (void *)MusicDir_StateFrame, (void *)MusicDir_ChaseBegins, (void *)MusicDir_Lost, (void *)MusicDir_Seen,
    (void *)MusicDir_Panic, (void *)MusicDir_PanicOver, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MusicDirBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MovieCopied_dtor, (void *)Scene_Update,
    (void *)MovieCopied_Entry, (void *)Movie_Finish, (void *)MovieCopied_Shot, (void *)Movie_Frame,
    (void *)MovieCopied_TakeFrame, (void *)MovieCopied_Draw, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)DepthRange_dtor, (void *)DepthRange_Start,
    (void *)DepthRange_Update, (void *)DepthRange_Draw, (void *)DepthRange_SetParams, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)DofDrawer_dtor, (void *)func_002C6650,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MovieAdded_dtor, (void *)Scene_Update,
    (void *)MovieAdded_Entry, (void *)Movie_Finish, (void *)Movie_Shot, (void *)Movie_Frame,
    (void *)MovieAdded_TakeFrame, (void *)MovieAdded_Draw, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MovieScene_dtor, (void *)Scene_Update,
    (void *)MovieScene_Entry, (void *)Movie_Finish, (void *)Movie_Shot, (void *)Movie_Frame,
    (void *)Movie_TakeFrame, (void *)Movie_Draw, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)PlacedObjects_ClearAll, (void *)PlacedObjects_CallDtor,
    (void *)PlacedObjects_SetPair, (void *)PlacedObjects_Create, (void *)PlacedObjects_Named, (void *)PlacedObjects_RecordEntry,
    (void *)PlacedObjects_ReleaseAll, (void *)PlacedObjects_Draw, (void *)PlacedObjects_Update, (void *)PlacedObjects_Noop34,
    (void *)PlacedObjects_Sum, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Cutscene_Destroy, (void *)Cutscene_Call8,
    (void *)Cutscene_RunState, (void *)Cutscene_Update, (void *)Cutscene_SetScript, (void *)Cutscene_ShotAt,
    (void *)Cutscene_IntoShot, (void *)Cutscene_NextShot, (void *)Cutscene_Get28, (void *)Cutscene_Status,
    (void *)Cutscene_PushC, (void *)Cutscene_Frame, (void *)Cutscene_Set38, (void *)Cutscene_SetHold,
    (void *)Cutscene_SetNoEnd, (void *)Cutscene_EntryDone, (void *)Cutscene_End, (void *)Cutscene_FrameSignals,
    (void *)Cutscene_SignalBits, (void *)Cutscene_SignalCount, (void *)Cutscene_Get206, (void *)Cutscene_Get216,
    (void *)Cutscene_NextEvent, (void *)Cutscene_Letterbox, (void *)Cutscene_NearEnd, (void *)Cutscene_Mark82,
    (void *)Cutscene_Signal2, (void *)Cutscene_Signals34, (void *)Cutscene_Mark83, (void *)Cutscene_SetLetterboxOff,
    (void *)Cutscene_SlotGroup, (void *)Cutscene_Slot84, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room2F_dtor, (void *)Room2F_EnterScript,
    (void *)Room2F_Phase1Script, (void *)Room2F_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room2F_Phase5Script, (void *)Room2F_ActionScript, (void *)RoomBase_Command, (void *)Room2F_Condition,
    (void *)Room2F_CharEnterScript, (void *)Room2F_ObjectName, (void *)Room2F_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room30_dtor, (void *)Room30_EnterScript,
    (void *)Room30_Phase1Script, (void *)Room30_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room30_Phase5Script, (void *)Room30_ActionScript, (void *)RoomBase_Command, (void *)Room30_Condition,
    (void *)Room30_CharEnterScript, (void *)Room30_ObjectName, (void *)Room30_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item00_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item00_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item00_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item01_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)Item01_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item01_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)Item01_CountUp, (void *)Item01_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item02_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item02_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item02_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item03_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item03_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item03_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item04_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item04_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item04_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item05_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item05_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item05_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Debilitas3_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Debilitas3_Update, (void *)Pursuer_LeaveScreen, (void *)Pursuer_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Debilitas3_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Pursuer_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Debilitas3_DoorOffset,
    (void *)Debilitas3_TurnRate, (void *)Debilitas3_TurnRateFast, (void *)Debilitas3_BlockFlags, (void *)Debilitas3_GoTo,
    (void *)Debilitas3_HeadForFiona, (void *)Debilitas3_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)Debilitas3_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)Debilitas3_Setup, (void *)Debilitas3_ModelFiles, (void *)Debilitas3_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Pursuer_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Debilitas3_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Debilitas3_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Debilitas3_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Debilitas3_Stairs, (void *)Debilitas3_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Debilitas3_FollowPathExit, (void *)Debilitas3_Arrived, (void *)Debilitas3_WalkToExit,
    (void *)Debilitas3_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Debilitas3_Timer10s, (void *)Pursuer_Timer15s, (void *)Debilitas3_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Debilitas3_ActionOffsets, (void *)Debilitas3_LookFrames,
    (void *)Debilitas3_LookSwing, (void *)Debilitas3_ReachFiona, (void *)Debilitas3_Dist2E8, (void *)Debilitas3_AttackAngle,
    (void *)Debilitas3_ReachHewie, (void *)Debilitas3_AttackRange, (void *)Debilitas3_SpeedTop, (void *)Debilitas3_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Debilitas3_AttackAnimA,
    (void *)Debilitas3_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Progress73EC80_dtor, (void *)Overlay_Draw,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Game_dtor, (void *)Game_Resident1AC0,
    (void *)Game_Resident2AC0, (void *)Game_ResidentB2C0, (void *)Game_Resident102C0, (void *)Game_Resident312C0,
    (void *)Game_Resident38AC0, (void *)Game_StartNextScene, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)AvoidPromptBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room17_dtor, (void *)Room17_EnterScript,
    (void *)Room17_Phase1Script, (void *)Room17_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room17_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room17_CharEnterScript, (void *)Room17_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemClassF430_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassF430_Kind, (void *)ItemClassF430_Actions, (void *)ItemClassF430_IsCounted, (void *)PoolEntry_SortKey,
    (void *)ItemClassF430_Get20, (void *)PoolEntry_Data, (void *)ItemClassF430_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemClassF430_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MusicStage1_dtor, (void *)MusicStage1_Load,
    (void *)MusicDir_BanksIn, (void *)MusicDir_Reset, (void *)MusicDir_Release, (void *)MusicDir_Hold,
    (void *)MusicDir_SetMusicVolume, (void *)MusicDir_SetProgressVolume, (void *)MusicDir_Volume4, (void *)MusicDir_Get12,
    (void *)MusicDir_VolumeC, (void *)MusicDir_Get14, (void *)MusicDir_GlobalVolumeTo, (void *)MusicDir_Start,
    (void *)MusicStage1_StageChans, (void *)MusicDir_SetVolume, (void *)MusicDir_FullVolume, (void *)MusicDir_Silence,
    (void *)MusicDir_StateFrame, (void *)MusicDir_ChaseBegins, (void *)MusicStage1_Lost, (void *)MusicStage1_Seen,
    (void *)MusicDir_Panic, (void *)MusicDir_PanicOver, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Rumble_dtor, (void *)Rumble_Reset,
    (void *)Rumble_Clear, (void *)Rumble_SetA, (void *)Rumble_SetB, (void *)Rumble_MoveB,
    (void *)Rumble_StartLists, (void *)Rumble_ListRunning, (void *)Rumble_Enable, (void *)Rumble_Set2C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Ball_dtor, (void *)Ball_Setup,
    (void *)Actor_Cleanup, (void *)Actor_LoadFiles, (void *)Actor_FilesLoading, (void *)Actor_FilesLoaded,
    (void *)Actor_Unload, (void *)Actor_RememberPos, (void *)Actor_Place, (void *)Ball_Draw,
    (void *)Ball_Frame, (void *)Actor_LeaveScreen, (void *)Actor_ShowUp, (void *)Actor_CanAct,
    (void *)Thing_PutAt, (void *)Ball_PutDown, (void *)Ball_LandTri, (void *)Ball_MotionState,
    (void *)Ball_Flying, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)EffectBase_dtor, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)EffectBase_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RisingSmoke_dtor, (void *)RisingSmoke_Start,
    (void *)RisingSmoke_Update, (void *)RisingSmoke_Draw, (void *)EffectBase_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)PlacedThings_New, (void *)PlacedThings_Get,
    (void *)PlacedThings_FirstOfKind, (void *)PlacedThings_AnotherCame, (void *)PlacedThings_Save, (void *)PlacedThings_Load,
    (void *)PlacedThings_PlaceKept, (void *)PlacedThings_Clear, (void *)PlacedThings_PoolCall, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)TvScreenB_dtor, (void *)TvScreenB_Start,
    (void *)TvScreenB_Update, (void *)TvScreenB_Draw, (void *)TvScreenB_SetParams, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemA0_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD640_Kind, (void *)ItemClassD640_Actions, (void *)ItemClassD640_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)ItemA0_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemA0_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemA1_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD640_Kind, (void *)ItemClassD640_Actions, (void *)ItemClassD640_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)ItemA1_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemA1_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Riccardo_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Riccardo_Update, (void *)Pursuer_LeaveScreen, (void *)Pursuer_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Pursuer_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Riccardo_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)Riccardo_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)Riccardo_ExitArg, (void *)Riccardo_Setup, (void *)Riccardo_ModelFiles, (void *)Riccardo_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Pursuer_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Riccardo_AttackTable, (void *)Pursuer_PickAttack, (void *)Riccardo_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Riccardo_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Riccardo_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Riccardo_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Riccardo_ReachHewie, (void *)Riccardo_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Riccardo_AttackAnimA,
    (void *)Riccardo_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Riccardo_SetRage,
    (void *)Riccardo_WalkAnim, (void *)Riccardo_StanceAnim, (void *)Riccardo_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Model_dtor, (void *)Model_Loaded,
    (void *)Model_Frame, (void *)Model_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)Model_Vt2C,
    (void *)Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)Model_Vt3C,
    (void *)Model_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)Model_PlantFeet,
    (void *)Model_FeetReplace, (void *)Model_FeetUp, (void *)Model_Vt58, (void *)Model_EaseTilt,
    (void *)Model_HeadPos, (void *)Model_FootDown, (void *)Model_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)Model_Vt74, (void *)Model_Vt78, (void *)Model_Vt7C,
    (void *)Model_HeadBone, (void *)Model_Part0, (void *)Model_Part1, (void *)Model_Part2,
    (void *)Model_Part3, (void *)Model_Vt94, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)Model_ModelFile, (void *)Model_MarkerFile, (void *)Model_Textures, (void *)Model_VtAC,
    (void *)Model_VtB0, (void *)Model_SecondaryMotion, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)CreatureA_dtor, (void *)CreatureA_Setup,
    (void *)CreatureA_Cleanup, (void *)Actor_LoadFiles, (void *)Actor_FilesLoading, (void *)CreatureA_FilesLoaded,
    (void *)CreatureA_Unload, (void *)Character_RememberPos, (void *)CreatureA_Place, (void *)CreatureA_Draw,
    (void *)CreatureA_Frame, (void *)CreatureA_FionaLeft, (void *)CreatureA_EnterRoom, (void *)CreatureA_IsUp,
    (void *)CreatureA_ModelMatrix, (void *)Character_Think, (void *)Character_PlaceModel, (void *)Character_Disable,
    (void *)Character_Enable, (void *)Character_LoadMessage, (void *)Character_Deactivate, (void *)CreatureA_Activate,
    (void *)Character_ResetBehaviour, (void *)CreatureA_PlaceInRoom, (void *)Character_EventConcerns, (void *)Character_SaveState,
    (void *)Character_LoadState, (void *)Character_AttackPoint, (void *)Character_LeftBehind, (void *)Character_BackToStance,
    (void *)Character_RegionFade, (void *)CreatureA_ActionState, (void *)CreatureA_Vt88, (void *)Character_BackToNormal,
    (void *)Character_EventReset, (void *)Character_TimerDown, (void *)Character_IsBusy, (void *)CreatureA_ComeAfterFiona,
    (void *)CreatureA_SetupKind, (void *)CreatureA_Save, (void *)CreatureA_SetSaveSlot, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)CreatureBase_dtor, (void *)CreatureBase_Setup,
    (void *)CreatureBase_Cleanup, (void *)Actor_LoadFiles, (void *)Actor_FilesLoading, (void *)Actor_FilesLoaded,
    (void *)Actor_Unload, (void *)Character_RememberPos, (void *)Character_Place, (void *)CreatureBase_Draw,
    (void *)CreatureBase_Frame, (void *)CreatureBase_FionaLeft, (void *)CreatureBase_EnterRoom, (void *)CreatureBase_IsUp,
    (void *)CreatureBase_ModelMatrix, (void *)Character_Think, (void *)Character_PlaceModel, (void *)Character_Disable,
    (void *)Character_Enable, (void *)Character_LoadMessage, (void *)Character_Deactivate, (void *)Character_Activate,
    (void *)Character_ResetBehaviour, (void *)0x00000000u, (void *)Character_EventConcerns, (void *)Character_SaveState,
    (void *)Character_LoadState, (void *)Character_AttackPoint, (void *)Character_LeftBehind, (void *)Character_BackToStance,
    (void *)Character_RegionFade, (void *)0x00000000u, (void *)Character_EventCommand, (void *)Character_BackToNormal,
    (void *)Character_EventReset, (void *)Character_TimerDown, (void *)Character_IsBusy, (void *)CreatureBase_ComeAfterFiona,
    (void *)CreatureBase_SetupKind, (void *)CreatureBase_Save, (void *)CreatureBase_SetSaveSlot, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Creatures_PoolCall, (void *)Creatures_ModelCall,
    (void *)Creatures_Get, (void *)Creatures_SendAfterFiona, (void *)Creatures_SetUpOne, (void *)Creatures_Keep,
    (void *)Creatures_MessageSlot, (void *)Creatures_ModelSet, (void *)Creatures_Remove, (void *)Creatures_Call2C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Helper46FC30_dtor, (void *)QuadDrawer_Start,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room46_dtor, (void *)Room46_EnterScript,
    (void *)Room46_Phase1Script, (void *)Room46_Phase2Script, (void *)Room46_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room46_Phase5Script, (void *)Room46_ActionScript, (void *)Room46_Command, (void *)Room46_Condition,
    (void *)Room46_CharEnterScript, (void *)Room46_ObjectName, (void *)Room46_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room102_dtor, (void *)Room102_EnterScript,
    (void *)Room102_Phase1Script, (void *)Room102_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room102_ActionScript, (void *)Room102_Command, (void *)RoomBase_Condition,
    (void *)Room102_CharEnterScript, (void *)Room102_ObjectName, (void *)Room102_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room103_dtor, (void *)Room103_EnterScript,
    (void *)Room103_Phase1Script, (void *)Room103_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room103_ActionScript, (void *)Room103_Command, (void *)RoomBase_Condition,
    (void *)Room103_CharEnterScript, (void *)Room103_ObjectName, (void *)Room103_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room104_dtor, (void *)Room104_EnterScript,
    (void *)Room104_Phase1Script, (void *)Room104_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room104_ActionScript, (void *)Room104_Command, (void *)RoomBase_Condition,
    (void *)Room104_CharEnterScript, (void *)Room104_ObjectName, (void *)Room104_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room105_dtor, (void *)Room105_EnterScript,
    (void *)Room105_Phase1Script, (void *)Room105_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room105_ActionScript, (void *)Room105_Command, (void *)RoomBase_Condition,
    (void *)Room105_CharEnterScript, (void *)Room105_ObjectName, (void *)Room105_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room106_dtor, (void *)Room106_EnterScript,
    (void *)Room106_Phase1Script, (void *)Room106_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room106_ActionScript, (void *)Room106_Command, (void *)RoomBase_Condition,
    (void *)Room106_CharEnterScript, (void *)Room106_ObjectName, (void *)Room106_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room107_dtor, (void *)Room107_EnterScript,
    (void *)Room107_Phase1Script, (void *)Room107_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room107_Phase5Script, (void *)Room107_ActionScript, (void *)Room107_Command, (void *)Room107_Condition,
    (void *)Room107_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room107_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room108_dtor, (void *)Room108_EnterScript,
    (void *)Room108_Phase1Script, (void *)Room108_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room108_Phase5Script, (void *)Room108_ActionScript, (void *)Room108_Command, (void *)RoomBase_Condition,
    (void *)Room108_CharEnterScript, (void *)Room108_ObjectName, (void *)Room108_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room109_dtor, (void *)Room109_EnterScript,
    (void *)Room109_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room109_Phase5Script, (void *)Room109_ActionScript, (void *)Room109_Command, (void *)RoomBase_Condition,
    (void *)Room109_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room109_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room10A_dtor, (void *)Room10A_EnterScript,
    (void *)Room10A_Phase1Script, (void *)Room10A_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room10A_Phase5Script, (void *)Room10A_ActionScript, (void *)Room10A_Command, (void *)Room10A_Condition,
    (void *)Room10A_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room10A_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room10B_dtor, (void *)Room10B_EnterScript,
    (void *)Room10B_Phase1Script, (void *)RoomBase_Phase2Script, (void *)Room10B_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room10B_ActionScript, (void *)Room10B_Command, (void *)Room10B_Condition,
    (void *)Room10B_CharEnterScript, (void *)Room10B_ObjectName, (void *)Room10B_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)EvEffect7F_dtor, (void *)EvEffect7F_Start,
    (void *)EvEffect7F_Update, (void *)EvEffect7F_Draw, (void *)EvEffect7F_SetParams, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SpriteBurst_dtor, (void *)SpriteBurst_Start,
    (void *)SpriteBurst_Update, (void *)SpriteBurst_Draw, (void *)SpriteBurst_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)EvEffect86_dtor, (void *)EvEffect86_Start,
    (void *)EvEffect86_Update, (void *)EvEffect86_Draw, (void *)EvEffect86_SetParams, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Effect6FF60_dtor, (void *)Effect6FF60_Start,
    (void *)Effect6FF60_Update, (void *)Effect6FF60_Draw, (void *)Effect6FF60_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind08_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind08_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind08_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)Kind08_Setup, (void *)Kind08_Table6C, (void *)Kind08_Table70,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind08_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SpringPart_Reset, (void *)SpringPart_NoopC,
    (void *)HangPoint_Frame, (void *)SpringPartBase_AdjustBone, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)DaniellaModel_dtor, (void *)DaniellaModel_Loaded,
    (void *)DaniellaModel_Frame, (void *)HumanModel_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)DaniellaModel_Vt2C,
    (void *)DaniellaModel_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)DaniellaModel_Vt3C,
    (void *)HumanModel_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)HumanModel_PlantFeet,
    (void *)Model_FeetReplace, (void *)HumanModel_FeetUp, (void *)HumanModel_Vt58, (void *)Model_EaseTilt,
    (void *)HumanModel_HeadPos, (void *)Model_FootDown, (void *)Model_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)HumanModel_Vt74, (void *)HumanModel_Vt78, (void *)HumanModel_Vt7C,
    (void *)HumanModel_HeadBone, (void *)DaniellaModel_Part0, (void *)DaniellaModel_Part1, (void *)DaniellaModel_Part2,
    (void *)DaniellaModel_Part3, (void *)Model_Vt94, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)Model_ModelFile, (void *)Model_MarkerFile, (void *)Model_Textures, (void *)Model_VtAC,
    (void *)Model_VtB0, (void *)DaniellaModel_SecondaryMotion, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Capsule_Push, (void *)Capsule_Update,
    (void *)0x00000000u, (void *)0x00000000u, (void *)BonePoint_dtor, (void *)BonePoint_Loaded,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SpringPart_Reset, (void *)SpringPart_NoopC,
    (void *)SpringPart_Noop10, (void *)SpringPartBase_AdjustBone, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SpringPart_Reset, (void *)SpringPart_NoopC,
    (void *)BoneHangPoint_Frame, (void *)SpringPartBase_AdjustBone, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item06_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item06_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item06_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SpringPart_Reset, (void *)SpringPart_NoopC,
    (void *)SprungPoint_Frame, (void *)SprungPoint_AdjustBone, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SpringPart_Reset, (void *)SpringPart_NoopC,
    (void *)HairPoint_Step, (void *)HairPoint_Pose, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RiccardoModel_dtor, (void *)RiccardoModel_Loaded,
    (void *)RiccardoModel_Frame, (void *)HumanModel_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)RiccardoModel_Vt2C,
    (void *)RiccardoModel_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)RiccardoModel_Vt3C,
    (void *)HumanModel_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)HumanModel_PlantFeet,
    (void *)Model_FeetReplace, (void *)HumanModel_FeetUp, (void *)HumanModel_Vt58, (void *)Model_EaseTilt,
    (void *)HumanModel_HeadPos, (void *)Model_FootDown, (void *)Model_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)HumanModel_Vt74, (void *)HumanModel_Vt78, (void *)HumanModel_Vt7C,
    (void *)HumanModel_HeadBone, (void *)RiccardoModel_Part0, (void *)RiccardoModel_Part1, (void *)RiccardoModel_Part2,
    (void *)RiccardoModel_Part3, (void *)Model_Vt94, (void *)RiccardoModel_Vt98, (void *)RiccardoModel_Vt9C,
    (void *)Model_ModelFile, (void *)Model_MarkerFile, (void *)Model_Textures, (void *)Model_VtAC,
    (void *)Model_VtB0, (void *)RiccardoModel_SecondaryMotion, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)EventHumanModel_dtor, (void *)EventHumanModel_Loaded,
    (void *)EventHumanModel_Frame, (void *)HumanModel_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)Model_Vt2C,
    (void *)Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)EventHumanModel_Springs,
    (void *)HumanModel_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)HumanModel_PlantFeet,
    (void *)Model_FeetReplace, (void *)HumanModel_FeetUp, (void *)HumanModel_Vt58, (void *)Model_EaseTilt,
    (void *)HumanModel_HeadPos, (void *)Model_FootDown, (void *)CharModel_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)HumanModel_Vt74, (void *)HumanModel_Vt78, (void *)HumanModel_Vt7C,
    (void *)HumanModel_HeadBone, (void *)EventHumanModel_Part0, (void *)EventHumanModel_Part1, (void *)EventHumanModel_Part2,
    (void *)EventHumanModel_Part3, (void *)EventHumanModel_Part4, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)EventHumanModel_ModelFile, (void *)CharModel_MarkerFile, (void *)EventHumanModel_Textures, (void *)EventHumanModel_Textures2,
    (void *)EventHumanModel_BufferSize, (void *)CharModel_SecondaryMotion, (void *)EventHumanModel_MotionTable, (void *)EventHumanModel_SetFlag,
    (void *)EventHumanModel_SetPoint, (void *)EventHumanModel_ShowParts, (void *)CharModel_SetVector, (void *)CharModel_VtCC,
    (void *)CharModel_VtD0, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)FionaModel_dtor, (void *)FionaModel_Loaded,
    (void *)FionaModel_Frame, (void *)HumanModel_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)Model_Vt2C,
    (void *)Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)FionaModel_Springs,
    (void *)HumanModel_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)HumanModel_PlantFeet,
    (void *)Model_FeetReplace, (void *)HumanModel_FeetUp, (void *)HumanModel_Vt58, (void *)Model_EaseTilt,
    (void *)HumanModel_HeadPos, (void *)Model_FootDown, (void *)CharModel_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)HumanModel_Vt74, (void *)HumanModel_Vt78, (void *)HumanModel_Vt7C,
    (void *)HumanModel_HeadBone, (void *)Model_Part0, (void *)Model_Part1, (void *)Model_Part2,
    (void *)Model_Part3, (void *)Model_Vt94, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)FionaModel_ModelFile, (void *)FionaModel_MarkerFile, (void *)FionaModel_Textures, (void *)FionaModel_Textures2,
    (void *)FionaModel_BufferSize, (void *)CharModel_SecondaryMotion, (void *)FionaModel_MotionTable, (void *)CharModel_SetFlag,
    (void *)CharModel_SetPoint, (void *)CharModel_ShowParts, (void *)FionaModel_SetVector, (void *)CharModel_VtCC,
    (void *)CharModel_VtD0, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SpringPart_Reset, (void *)SpringPart_NoopC,
    (void *)SwayPointA_Step, (void *)SwayPointA_Pose, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Lorenzo_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Lorenzo_Update, (void *)Pursuer_LeaveScreen, (void *)Pursuer_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Pursuer_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Lorenzo_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)Lorenzo_Setup, (void *)Lorenzo_ModelFiles, (void *)Lorenzo_MotionFiles,
    (void *)Lorenzo_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Pursuer_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Lorenzo_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Lorenzo_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Lorenzo_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Lorenzo_ReachHewie, (void *)Lorenzo_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RisingMotes_dtor, (void *)RisingMotes_Start,
    (void *)RisingMotes_Update, (void *)RisingMotes_Draw, (void *)EffectBase_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)OrangeSparks_dtor, (void *)OrangeSparks_Start,
    (void *)OrangeSparks_Update, (void *)OrangeSparks_Draw, (void *)OrangeSparks_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Debilitas2_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Debilitas2_Update, (void *)Pursuer_LeaveScreen, (void *)Pursuer_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Pursuer_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Debilitas2_DoorOffset,
    (void *)Debilitas2_TurnRate, (void *)Debilitas2_TurnRateFast, (void *)Debilitas2_BlockFlags, (void *)Debilitas2_GoTo,
    (void *)Debilitas2_HeadForFiona, (void *)Debilitas2_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)Debilitas2_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)Debilitas2_Setup, (void *)Debilitas2_ModelFiles, (void *)Debilitas2_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Pursuer_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Debilitas2_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Debilitas2_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Debilitas2_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Debilitas2_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Debilitas2_Stairs, (void *)Debilitas2_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Debilitas2_GoForFiona,
    (void *)Debilitas2_HeadingStep, (void *)Debilitas2_FreshStart, (void *)Debilitas2_CarryOn, (void *)Pursuer_Caught,
    (void *)Debilitas2_Timer10s, (void *)Pursuer_Timer15s, (void *)Debilitas2_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Debilitas2_ActionOffsets, (void *)Debilitas2_LookFrames,
    (void *)Debilitas2_LookSwing, (void *)Debilitas2_ReachFiona, (void *)Debilitas2_Dist2E8, (void *)Debilitas2_AttackAngle,
    (void *)Debilitas2_ReachHewie, (void *)Debilitas2_AttackRange, (void *)Debilitas2_SpeedTop, (void *)Debilitas2_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Debilitas2_AttackAnimA,
    (void *)Debilitas2_AttackAnimB, (void *)Debilitas2_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Debilitas2_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room27_dtor, (void *)Room27_EnterScript,
    (void *)Room27_Phase1Script, (void *)Room27_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room27_ActionScript, (void *)Room27_Command, (void *)RoomBase_Condition,
    (void *)Room27_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SinkingSprite_dtor, (void *)SinkingSprite_Start,
    (void *)SinkingSprite_Update, (void *)SinkingSprite_Draw, (void *)SinkingSprite_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SmokePuffs_dtor, (void *)SmokePuffs_Start,
    (void *)SmokePuffs_Update, (void *)SmokePuffs_Draw, (void *)SmokePuffs_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SpeckSwarm_dtor, (void *)SpeckSwarm_Start,
    (void *)SpeckSwarm_Update, (void *)SpeckSwarm_Draw, (void *)SpeckSwarm_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Splash_dtor, (void *)Splash_Start,
    (void *)Splash_Update, (void *)Splash_Draw, (void *)Splash_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MovieOwnBuf_dtor, (void *)Scene_Update,
    (void *)MovieOwnBuf_Entry, (void *)Movie_Finish, (void *)Movie_Shot, (void *)Movie_Frame,
    (void *)MovieOwnBuf_TakeFrame, (void *)MovieOwnBuf_Draw, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room2C_dtor, (void *)Room2C_EnterScript,
    (void *)Room2C_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room2C_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room2C_CharEnterScript, (void *)Room2C_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room2E_dtor, (void *)Room2E_EnterScript,
    (void *)Room2E_Phase1Script, (void *)Room2E_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room2E_Phase5Script, (void *)Room2E_ActionScript, (void *)Room2E_Command, (void *)RoomBase_Condition,
    (void *)Room2E_CharEnterScript, (void *)Room2E_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)BloodSpray_dtor, (void *)BloodSpray_Start,
    (void *)BloodSpray_Update, (void *)BloodSpray_Draw, (void *)BloodSpray_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room66_dtor, (void *)Room66_EnterScript,
    (void *)Room66_Phase1Script, (void *)Room66_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room66_Phase5Script, (void *)Room66_ActionScript, (void *)Room66_Command, (void *)Room66_Condition,
    (void *)Room66_CharEnterScript, (void *)Room66_ObjectName, (void *)Room66_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Obj470F90_dtor, (void *)Marker_Start,
    (void *)Marker_Update, (void *)Marker_Draw, (void *)Marker_SetParams, (void *)Marker_State,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item07_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item07_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item07_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Effect71000_dtor, (void *)Effect71000_Start,
    (void *)Effect71000_Update, (void *)Effect71000_Draw, (void *)Effect71000_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room55_dtor, (void *)Room55_EnterScript,
    (void *)Room55_Phase1Script, (void *)Room55_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room55_ActionScript, (void *)Room55_Command, (void *)RoomBase_Condition,
    (void *)Room55_CharEnterScript, (void *)Room55_ObjectName, (void *)Room55_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)DriftingFlecks_dtor, (void *)DriftingFlecks_Start,
    (void *)DriftingFlecks_Update, (void *)DriftingFlecks_Draw, (void *)DriftingFlecks_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item08_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item08_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item08_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item09_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item09_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item09_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item0A_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item0A_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item0A_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item0B_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item0B_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item0B_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item0C_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item0C_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item0C_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room63_dtor, (void *)Room63_EnterScript,
    (void *)Room63_Phase1Script, (void *)Room63_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room63_Phase5Script, (void *)Room63_ActionScript, (void *)Room63_Command, (void *)RoomBase_Condition,
    (void *)Room63_CharEnterScript, (void *)Room63_ObjectName, (void *)Room63_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room62_dtor, (void *)Room62_EnterScript,
    (void *)Room62_Phase1Script, (void *)Room62_Phase2Script, (void *)Room62_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room62_ActionScript, (void *)Room62_Command, (void *)RoomBase_Condition,
    (void *)Room62_CharEnterScript, (void *)Room62_ObjectName, (void *)Room62_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind09_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Kind09_Update, (void *)Pursuer_LeaveScreen, (void *)Kind09_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind09_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)func_00308FE0, (void *)Kind09_ModelFiles, (void *)Kind09_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind09_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Kind09_Behaviour25C,
    (void *)Kind09_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Kind09_BehaviourIdle, (void *)Kind09_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Lorenzo2_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Lorenzo2_Update, (void *)Pursuer_LeaveScreen, (void *)Pursuer_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Pursuer_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)Lorenzo2_Setup, (void *)Lorenzo2_ModelFiles, (void *)Lorenzo2_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Lorenzo2_InStance2Anim,
    (void *)Pursuer_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Lorenzo2_AttackTable, (void *)Pursuer_PickAttack, (void *)Lorenzo2_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Lorenzo2_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Lorenzo2_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Lorenzo2_ReachHewie, (void *)Lorenzo2_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Lorenzo2_FrightSeen, (void *)Lorenzo2_FrightAttack, (void *)Lorenzo2_ThreatAmount, (void *)Lorenzo2_AttackAnimA,
    (void *)Lorenzo2_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Lorenzo2_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Lorenzo2_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind12_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Kind12_Update, (void *)Pursuer_LeaveScreen, (void *)Kind12_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Kind12_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind12_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)Kind12_Setup, (void *)Kind12_ModelFiles, (void *)Kind12_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Kind12_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind12_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Kind12_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Kind12_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Kind12_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Kind12_ReachHewie, (void *)Kind12_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind09Model_dtor, (void *)Kind09Model_Loaded,
    (void *)func_0030D9E0, (void *)HumanModel_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)Model_Vt2C,
    (void *)Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)Kind09Model_Vt3C,
    (void *)HumanModel_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)HumanModel_PlantFeet,
    (void *)Model_FeetReplace, (void *)HumanModel_FeetUp, (void *)HumanModel_Vt58, (void *)Model_EaseTilt,
    (void *)HumanModel_HeadPos, (void *)Model_FootDown, (void *)Model_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)HumanModel_Vt74, (void *)HumanModel_Vt78, (void *)HumanModel_Vt7C,
    (void *)HumanModel_HeadBone, (void *)Kind09Model_Part0, (void *)Kind09Model_Part1, (void *)Kind09Model_Part2,
    (void *)Kind09Model_Part3, (void *)Model_Vt94, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)Model_ModelFile, (void *)Model_MarkerFile, (void *)Model_Textures, (void *)Model_VtAC,
    (void *)Model_VtB0, (void *)Kind09Model_SecondaryMotion, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Lorenzo2Model_dtor, (void *)Lorenzo2Model_Loaded,
    (void *)Lorenzo2Model_Frame, (void *)HumanModel_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)Model_Vt2C,
    (void *)Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)Lorenzo2Model_Vt3C,
    (void *)HumanModel_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)HumanModel_PlantFeet,
    (void *)Model_FeetReplace, (void *)HumanModel_FeetUp, (void *)HumanModel_Vt58, (void *)Model_EaseTilt,
    (void *)HumanModel_HeadPos, (void *)Model_FootDown, (void *)Model_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)HumanModel_Vt74, (void *)HumanModel_Vt78, (void *)HumanModel_Vt7C,
    (void *)HumanModel_HeadBone, (void *)Lorenzo2Model_Part0, (void *)Lorenzo2Model_Part1, (void *)Lorenzo2Model_Part2,
    (void *)Lorenzo2Model_Part3, (void *)Model_Vt94, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)Model_ModelFile, (void *)Model_MarkerFile, (void *)Model_Textures, (void *)Model_VtAC,
    (void *)Model_VtB0, (void *)Lorenzo2Model_SecondaryMotion, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)LorenzoModel_dtor, (void *)LorenzoModel_Loaded,
    (void *)LorenzoModel_Frame, (void *)LorenzoModel_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)LorenzoModel_SetMatrix, (void *)Model_Vt2C,
    (void *)Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)LorenzoModel_Vt3C,
    (void *)LorenzoModel_BodyFrames, (void *)LorenzoModel_SlopeKeep, (void *)Model_FloorLevel, (void *)Model_PlantFeet,
    (void *)Model_FeetReplace, (void *)Model_FeetUp, (void *)LorenzoModel_Vt58, (void *)Model_EaseTilt,
    (void *)LorenzoModel_HeadPos, (void *)Model_FootDown, (void *)Model_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)Model_Vt74, (void *)Model_Vt78, (void *)Model_Vt7C,
    (void *)LorenzoModel_HeadBone, (void *)LorenzoModel_Part0, (void *)LorenzoModel_Part1, (void *)LorenzoModel_Part2,
    (void *)LorenzoModel_Part3, (void *)Model_Vt94, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)Model_ModelFile, (void *)Model_MarkerFile, (void *)Model_Textures, (void *)Model_VtAC,
    (void *)Model_VtB0, (void *)LorenzoModel_SecondaryMotion, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room68_dtor, (void *)Room68_EnterScript,
    (void *)Room68_Phase1Script, (void *)Room68_Phase2Script, (void *)RoomBase_Phase3Script, (void *)Room68_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room68_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room68_CharEnterScript, (void *)Room68_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room47_dtor, (void *)Room47_EnterScript,
    (void *)Room47_Phase1Script, (void *)Room47_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room47_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room47_CharEnterScript, (void *)Room47_ObjectName, (void *)Room47_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room48_dtor, (void *)Room48_EnterScript,
    (void *)Room48_Phase1Script, (void *)Room48_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room48_Phase5Script, (void *)Room48_ActionScript, (void *)Room48_Command, (void *)Room48_Condition,
    (void *)Room48_CharEnterScript, (void *)Room48_ObjectName, (void *)Room48_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room53_dtor, (void *)Room53_EnterScript,
    (void *)Room53_Phase1Script, (void *)Room53_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room53_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room53_CharEnterScript, (void *)Room53_ObjectName, (void *)Room53_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room54_dtor, (void *)Room54_EnterScript,
    (void *)Room54_Phase1Script, (void *)Room54_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room54_ActionScript, (void *)Room54_Command, (void *)RoomBase_Condition,
    (void *)Room54_CharEnterScript, (void *)Room54_ObjectName, (void *)Room54_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room60_dtor, (void *)Room60_EnterScript,
    (void *)Room60_Phase1Script, (void *)Room60_Phase2Script, (void *)Room60_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room60_ActionScript, (void *)Room60_Command, (void *)RoomBase_Condition,
    (void *)Room60_CharEnterScript, (void *)Room60_ObjectName, (void *)Room60_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room61_dtor, (void *)Room61_EnterScript,
    (void *)Room61_Phase1Script, (void *)Room61_Phase2Script, (void *)Room61_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room61_Phase5Script, (void *)Room61_ActionScript, (void *)Room61_Command, (void *)RoomBase_Condition,
    (void *)Room61_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room61_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind13_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind13_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind13_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)func_00311AD0, (void *)Kind13_ModelFiles, (void *)Kind13_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind13_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SpringPart_Reset, (void *)SpringPart_NoopC,
    (void *)SwayPointB_Step, (void *)SwayPointB_Pose, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)CreatureVanish_dtor, (void *)CreatureVanish_Start,
    (void *)CreatureVanish_Update, (void *)CreatureVanish_Draw, (void *)CreatureVanish_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)LoopingSprite_dtor, (void *)LoopingSprite_Start,
    (void *)LoopingSprite_Update, (void *)LoopingSprite_Draw, (void *)LoopingSprite_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind14_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind14_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind14_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)Kind14_Setup, (void *)Kind14_ModelFiles, (void *)Kind14_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind14_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Effect726E0_dtor, (void *)Effect726E0_Start,
    (void *)Effect726E0_Update, (void *)Effect726E0_Draw, (void *)Effect726E0_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind14Model_dtor, (void *)Kind14Model_Loaded,
    (void *)Kind14Model_Frame, (void *)Model_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)Model_Vt2C,
    (void *)Model_Vt30, (void *)Model_Set878, (void *)Kind14Model_Draw, (void *)Model_Vt3C,
    (void *)Model_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)Model_PlantFeet,
    (void *)Model_FeetReplace, (void *)Model_FeetUp, (void *)Model_Vt58, (void *)Model_EaseTilt,
    (void *)Model_HeadPos, (void *)Model_FootDown, (void *)Model_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)Model_Vt74, (void *)Model_Vt78, (void *)Model_Vt7C,
    (void *)Model_HeadBone, (void *)Model_Part0, (void *)Model_Part1, (void *)Model_Part2,
    (void *)Model_Part3, (void *)Model_Vt94, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)Model_ModelFile, (void *)Model_MarkerFile, (void *)Model_Textures, (void *)Model_VtAC,
    (void *)Model_VtB0, (void *)Kind14Model_SecondaryMotion, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)StrandSplash_dtor, (void *)StrandSplash_Start,
    (void *)StrandSplash_Update, (void *)StrandSplash_Draw, (void *)StrandSplash_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Thing01_dtor, (void *)Thing01_Setup,
    (void *)Actor_Cleanup, (void *)Actor_LoadFiles, (void *)Actor_FilesLoading, (void *)Actor_FilesLoaded,
    (void *)Actor_Unload, (void *)Actor_RememberPos, (void *)Actor_Place, (void *)Thing01_Draw,
    (void *)Thing01_Frame, (void *)Actor_LeaveScreen, (void *)Actor_ShowUp, (void *)Actor_CanAct,
    (void *)Thing_PutAt, (void *)Thing01_PutDown, (void *)Thing01_LandTri, (void *)Thing01_MotionState,
    (void *)Thing01_Flying, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Thing02_dtor, (void *)Thing02_Setup,
    (void *)Actor_Cleanup, (void *)Actor_LoadFiles, (void *)Actor_FilesLoading, (void *)Actor_FilesLoaded,
    (void *)Actor_Unload, (void *)Actor_RememberPos, (void *)Actor_Place, (void *)Thing02_Draw,
    (void *)Thing02_Frame, (void *)Actor_LeaveScreen, (void *)Actor_ShowUp, (void *)Actor_CanAct,
    (void *)Thing_PutAt, (void *)ThingShared_PutDown, (void *)ThingShared_LandTri, (void *)ThingShared_MotionState,
    (void *)ThingShared_Flying, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind16_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind16_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind16_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)func_00315C60, (void *)Kind16_ModelFiles, (void *)Kind16_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind16_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SpringPart_Reset, (void *)SpringPart_NoopC,
    (void *)HairPoint2_Frame, (void *)HairPoint2_AdjustBone, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)FloorSplat_dtor, (void *)FloorSplat_Start,
    (void *)FloorSplat_Update, (void *)FloorSplat_Draw, (void *)FloorSplat_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SpringPart_Reset, (void *)SpringPart_NoopC,
    (void *)HangingPart_Step, (void *)SpringPartBase_AdjustBone, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind17_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind17_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind17_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)func_00316BE0, (void *)Kind17_ModelFiles, (void *)Kind17_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind17_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Obj472F60_dtor, (void *)Reflection_Start,
    (void *)Reflection_Update, (void *)Reflection_Draw, (void *)Reflection_SetParams, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item0D_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item0D_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item0D_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item0E_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item0E_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item0E_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item0F_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item0F_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item0F_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item10_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item10_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item10_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item11_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item11_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item11_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind18_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind18_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind18_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)func_0031D820, (void *)Kind18_ModelFiles, (void *)Kind18_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind18_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)AvoidPrompt_dtor, (void *)AvoidPrompt_Show,
    (void *)AvoidPrompt_DrawPart, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room31_dtor, (void *)Room31_EnterScript,
    (void *)Room31_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room31_ActionScript, (void *)Room31_Command, (void *)RoomBase_Condition,
    (void *)Room31_CharEnterScript, (void *)Room31_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind19_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind19_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind19_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)func_0031E6F0, (void *)Kind19_ModelFiles, (void *)Kind19_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind19_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Effect737D0_dtor, (void *)Effect737D0_Start,
    (void *)Effect737D0_Update, (void *)Effect737D0_Draw, (void *)Effect737D0_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SpringPart_Reset, (void *)SpringPart_NoopC,
    (void *)Part50_Step, (void *)SpringPartBase_AdjustBone, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SpringPart_Reset, (void *)SpringPart_NoopC,
    (void *)Part60_Step, (void *)SpringPartBase_AdjustBone, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MusicStage2_dtor, (void *)MusicStage2_Load,
    (void *)MusicDir_BanksIn, (void *)MusicDir_Reset, (void *)MusicDir_Release, (void *)MusicDir_Hold,
    (void *)MusicDir_SetMusicVolume, (void *)MusicDir_SetProgressVolume, (void *)MusicDir_Volume4, (void *)MusicDir_Get12,
    (void *)MusicDir_VolumeC, (void *)MusicDir_Get14, (void *)MusicDir_GlobalVolumeTo, (void *)MusicDir_Start,
    (void *)MusicStage2_StageChans, (void *)MusicDir_SetVolume, (void *)MusicDir_FullVolume, (void *)MusicDir_Silence,
    (void *)MusicDir_StateFrame, (void *)MusicDir_ChaseBegins, (void *)MusicStage2_Lost, (void *)MusicStage2_Seen,
    (void *)MusicDir_Panic, (void *)MusicDir_PanicOver, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)TintStalker_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)TintStalker_Draw,
    (void *)TintStalker_Update, (void *)Pursuer_LeaveScreen, (void *)Pursuer_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)TintStalker_Disable,
    (void *)TintStalker_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)TintStalker_Reset,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Pursuer_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)TintStalker_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)TintStalker_BreakDoor, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)TintStalker_EndingDoor, (void *)TintStalker_Setup, (void *)TintStalker_Table, (void *)TintStalker_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Pursuer_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)TintStalker_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)TintStalker_DoorDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)TintStalker_SetDelay,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)TintStalker_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)TintStalker_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)TintStalker_FrightSeen, (void *)TintStalker_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)TintStalker_AttackAnimA,
    (void *)TintStalker_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)TintStalker_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind23Model_dtor, (void *)Kind23Model_Loaded,
    (void *)func_00320D60, (void *)HumanModel_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)Kind23Model_Vt2C,
    (void *)Kind23Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)Kind23Model_Vt3C,
    (void *)HumanModel_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)HumanModel_PlantFeet,
    (void *)Model_FeetReplace, (void *)HumanModel_FeetUp, (void *)HumanModel_Vt58, (void *)Model_EaseTilt,
    (void *)HumanModel_HeadPos, (void *)Model_FootDown, (void *)Model_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)HumanModel_Vt74, (void *)HumanModel_Vt78, (void *)HumanModel_Vt7C,
    (void *)HumanModel_HeadBone, (void *)Kind23Model_Part0, (void *)Kind23Model_Part1, (void *)Kind23Model_Part2,
    (void *)Kind23Model_Part3, (void *)Model_Vt94, (void *)Kind23Model_Vt98, (void *)Kind23Model_Vt9C,
    (void *)Model_ModelFile, (void *)Model_MarkerFile, (void *)Model_Textures, (void *)Model_VtAC,
    (void *)Model_VtB0, (void *)Kind23Model_SecondaryMotion, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room32_dtor, (void *)Room32_EnterScript,
    (void *)Room32_Phase1Script, (void *)Room32_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room32_ActionScript, (void *)Room32_Command, (void *)RoomBase_Condition,
    (void *)Room32_CharEnterScript, (void *)Room32_ObjectName, (void *)Room32_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind24_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind24_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind24_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)func_00321770, (void *)Kind24_ModelFiles, (void *)Kind24_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind24_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room2AWisps_dtor, (void *)Room2AWisps_Start,
    (void *)Room2AWisps_Update, (void *)Room2AWisps_Draw, (void *)Room2AWisps_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SynthBase_Destroy, (void *)SynthBase_Reset,
    (void *)SynthBase_Update, (void *)SynthBase_Noop14, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SynthPot_dtor, (void *)SynthPot_Start,
    (void *)SynthPot_Update, (void *)SynthPot_Noop14, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SlotMachine_dtor, (void *)SlotMachine_Start,
    (void *)SlotMachine_Update, (void *)SlotMachine_Noop14, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)CreatureB_dtor, (void *)CreatureB_Setup,
    (void *)CreatureB_Cleanup, (void *)Actor_LoadFiles, (void *)Actor_FilesLoading, (void *)CreatureB_FilesLoaded,
    (void *)CreatureB_Unload, (void *)Character_RememberPos, (void *)CreatureB_Place, (void *)CreatureB_Draw,
    (void *)CreatureB_Frame, (void *)CreatureB_FionaLeft, (void *)CreatureB_EnterRoom, (void *)CreatureB_IsUp,
    (void *)CreatureB_ModelMatrix, (void *)Character_Think, (void *)Character_PlaceModel, (void *)Character_Disable,
    (void *)Character_Enable, (void *)Character_LoadMessage, (void *)Character_Deactivate, (void *)CreatureB_Activate,
    (void *)Character_ResetBehaviour, (void *)CreatureB_PlaceInRoom, (void *)Character_EventConcerns, (void *)Character_SaveState,
    (void *)Character_LoadState, (void *)Character_AttackPoint, (void *)Character_LeftBehind, (void *)Character_BackToStance,
    (void *)CreatureB_DrawLight, (void *)CreatureB_ActionState, (void *)CreatureB_Vt88, (void *)Character_BackToNormal,
    (void *)Character_EventReset, (void *)Character_TimerDown, (void *)Character_IsBusy, (void *)CreatureB_ComeAfterFiona,
    (void *)CreatureB_SetupKind, (void *)CreatureB_Save, (void *)CreatureB_SetSaveSlot, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind25_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind25_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind25_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)func_0032C370, (void *)Kind25_ModelFiles, (void *)Kind25_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind25_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind33Model_dtor, (void *)Model_Loaded,
    (void *)Model_Frame, (void *)Model_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)Model_Vt2C,
    (void *)Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)Model_Vt3C,
    (void *)Kind33Model_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)Model_PlantFeet,
    (void *)Model_FeetReplace, (void *)Model_FeetUp, (void *)Model_Vt58, (void *)Model_EaseTilt,
    (void *)Model_HeadPos, (void *)Model_FootDown, (void *)Model_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)Model_Vt74, (void *)Model_Vt78, (void *)Model_Vt7C,
    (void *)Model_HeadBone, (void *)Kind33Model_Part0, (void *)Kind33Model_Part1, (void *)Kind33Model_Part2,
    (void *)Kind33Model_Part3, (void *)Model_Vt94, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)Model_ModelFile, (void *)Model_MarkerFile, (void *)Model_Textures, (void *)Model_VtAC,
    (void *)Model_VtB0, (void *)Kind33Model_SecondaryMotion, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room86_dtor, (void *)Room86_EnterScript,
    (void *)Room86_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room86_ActionScript, (void *)Room86_Command, (void *)RoomBase_Condition,
    (void *)Room86_CharEnterScript, (void *)Room86_ObjectName, (void *)Room86_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind21_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind21_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind21_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)func_00179AB0, (void *)Kind21_ModelFiles, (void *)Kind21_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind21_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind20_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind20_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind20_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)func_0032CAF0, (void *)Kind20_ModelFiles, (void *)Kind20_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind20_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item14_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item14_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item14_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind26_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind26_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind26_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)func_0032CF30, (void *)Kind26_ModelFiles, (void *)Kind26_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind26_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomC0_dtor, (void *)RoomC0_EnterScript,
    (void *)RoomC0_Phase1Script, (void *)RoomC0_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomC0_ActionScript, (void *)RoomC0_Command, (void *)RoomC0_Condition,
    (void *)RoomC0_CharEnterScript, (void *)RoomC0_ObjectName, (void *)RoomC0_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MovieSmall_dtor, (void *)Scene_Update,
    (void *)MovieSmall_Entry, (void *)Movie_Finish, (void *)Movie_Shot, (void *)Movie_Frame,
    (void *)MovieSmall_TakeFrame, (void *)MovieSmall_Draw, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ThingBurst_dtor, (void *)ThingBurst_Start,
    (void *)ThingBurst_Update, (void *)ThingBurst_Draw, (void *)ThingBurst_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind27_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Kind27_Update, (void *)Pursuer_LeaveScreen, (void *)Pursuer_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Pursuer_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Kind27_DoorOffset,
    (void *)Kind27_TurnRate, (void *)Kind27_TurnRateFast, (void *)Kind27_BlockFlags, (void *)Kind27_GoTo,
    (void *)Kind27_HeadForFiona, (void *)Kind27_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)Kind27_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)Kind27_Setup, (void *)Kind27_ModelFiles, (void *)Kind27_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Kind27_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Pursuer_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Kind27_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Kind27_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Kind27_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)func_00330780, (void *)func_00330770, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Kind27_FollowPathExit, (void *)Kind27_Arrived, (void *)Kind27_WalkToExit,
    (void *)Kind27_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Kind27_Timer10s, (void *)Pursuer_Timer15s, (void *)Kind27_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Kind27_ActionOffsets, (void *)Kind27_LookFrames,
    (void *)Kind27_LookSwing, (void *)Kind27_ReachFiona, (void *)Kind27_Dist2E8, (void *)Kind27_AttackAngle,
    (void *)Kind27_ReachHewie, (void *)Kind27_AttackRange, (void *)Kind27_SpeedTop, (void *)Kind27_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Kind27_AttackAnimA,
    (void *)Kind27_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item42_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassC920_Kind, (void *)Item42_Actions, (void *)ItemClassC920_IsCounted, (void *)Item42_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item42_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item42_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item43_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassC920_Kind, (void *)Item43_Actions, (void *)ItemClassC920_IsCounted, (void *)Item43_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item43_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item43_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item44_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassC920_Kind, (void *)Item44_Actions, (void *)ItemClassC920_IsCounted, (void *)Item44_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item44_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item44_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item45_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassC920_Kind, (void *)Item45_Actions, (void *)ItemClassC920_IsCounted, (void *)Item45_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item45_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item45_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item46_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassC920_Kind, (void *)Item46_Actions, (void *)ItemClassC920_IsCounted, (void *)Item46_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item46_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item46_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item47_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassC920_Kind, (void *)Item47_Actions, (void *)ItemClassC920_IsCounted, (void *)Item47_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item47_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item47_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item48_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassC920_Kind, (void *)Item48_Actions, (void *)ItemClassC920_IsCounted, (void *)Item48_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item48_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item48_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item49_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassC920_Kind, (void *)Item49_Actions, (void *)ItemClassC920_IsCounted, (void *)Item49_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item49_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item49_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item4A_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassC920_Kind, (void *)Item4A_Actions, (void *)ItemClassC920_IsCounted, (void *)Item4A_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item4A_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item4A_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item4B_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassC920_Kind, (void *)Item4B_Actions, (void *)ItemClassC920_IsCounted, (void *)Item4B_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item4B_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item4B_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item4C_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassC920_Kind, (void *)Item4C_Actions, (void *)ItemClassC920_IsCounted, (void *)Item4C_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item4C_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item4C_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item60_dtor, (void *)PoolEntry_Id,
    (void *)ItemClass6B00_Kind, (void *)ItemClass6B00_Actions, (void *)ItemClass6B00_IsCounted, (void *)Item60_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item60_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item60_Use,
    (void *)ItemClass6B00_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item61_dtor, (void *)PoolEntry_Id,
    (void *)ItemClass6B00_Kind, (void *)ItemClass6B00_Actions, (void *)ItemClass6B00_IsCounted, (void *)Item61_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item61_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item61_Use,
    (void *)ItemClass6B00_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item62_dtor, (void *)PoolEntry_Id,
    (void *)ItemClass6B00_Kind, (void *)ItemClass6B00_Actions, (void *)ItemClass6B00_IsCounted, (void *)Item62_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item62_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item62_Use,
    (void *)ItemClass6B00_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item63_dtor, (void *)PoolEntry_Id,
    (void *)ItemClass6B00_Kind, (void *)Item63_Actions, (void *)ItemClass6B00_IsCounted, (void *)Item63_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item63_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item63_Use,
    (void *)ItemClass6B00_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item64_dtor, (void *)PoolEntry_Id,
    (void *)ItemClass6B00_Kind, (void *)Item64_Actions, (void *)ItemClass6B00_IsCounted, (void *)Item64_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item64_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item64_Use,
    (void *)ItemClass6B00_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item65_dtor, (void *)PoolEntry_Id,
    (void *)ItemClass6B00_Kind, (void *)Item65_Actions, (void *)ItemClass6B00_IsCounted, (void *)Item65_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item65_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item65_Use,
    (void *)ItemClass6B00_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item66_dtor, (void *)PoolEntry_Id,
    (void *)ItemClass6B00_Kind, (void *)Item66_Actions, (void *)ItemClass6B00_IsCounted, (void *)Item66_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item66_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item66_Use,
    (void *)ItemClass6B00_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Thing03_dtor, (void *)Thing03_Setup,
    (void *)Actor_Cleanup, (void *)Actor_LoadFiles, (void *)Actor_FilesLoading, (void *)Actor_FilesLoaded,
    (void *)Actor_Unload, (void *)Actor_RememberPos, (void *)Actor_Place, (void *)Thing03_Draw,
    (void *)Thing03_Frame, (void *)Actor_LeaveScreen, (void *)Actor_ShowUp, (void *)Actor_CanAct,
    (void *)Thing_PutAt, (void *)Thing03_PutDown, (void *)Thing03_LandTri, (void *)Thing03_MotionState,
    (void *)Thing03_Flying, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Thing06_dtor, (void *)Thing06_Setup,
    (void *)Actor_Cleanup, (void *)Actor_LoadFiles, (void *)Actor_FilesLoading, (void *)Actor_FilesLoaded,
    (void *)Actor_Unload, (void *)Actor_RememberPos, (void *)Actor_Place, (void *)Thing06_Draw,
    (void *)Thing06_Frame, (void *)Actor_LeaveScreen, (void *)Actor_ShowUp, (void *)Actor_CanAct,
    (void *)Thing_PutAt, (void *)Thing06_PutDown, (void *)Thing06_LandTri, (void *)Thing06_MotionState,
    (void *)Thing06_Flying, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Thing05_dtor, (void *)Thing05_Setup,
    (void *)Actor_Cleanup, (void *)Actor_LoadFiles, (void *)Actor_FilesLoading, (void *)Actor_FilesLoaded,
    (void *)Actor_Unload, (void *)Actor_RememberPos, (void *)Actor_Place, (void *)Thing05_Draw,
    (void *)Thing05_Frame, (void *)Actor_LeaveScreen, (void *)Actor_ShowUp, (void *)Actor_CanAct,
    (void *)Thing_PutAt, (void *)ThingShared_PutDown, (void *)ThingShared_LandTri, (void *)ThingShared_MotionState,
    (void *)ThingShared_Flying, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Thing07_dtor, (void *)Thing07_Setup,
    (void *)Actor_Cleanup, (void *)Actor_LoadFiles, (void *)Actor_FilesLoading, (void *)Actor_FilesLoaded,
    (void *)Actor_Unload, (void *)Actor_RememberPos, (void *)Actor_Place, (void *)Thing07_Draw,
    (void *)Thing07_Frame, (void *)Actor_LeaveScreen, (void *)Actor_ShowUp, (void *)Actor_CanAct,
    (void *)Thing_PutAt, (void *)ThingShared_PutDown, (void *)ThingShared_LandTri, (void *)ThingShared_MotionState,
    (void *)ThingShared_Flying, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Thing08_dtor, (void *)Thing08_Setup,
    (void *)Actor_Cleanup, (void *)Actor_LoadFiles, (void *)Actor_FilesLoading, (void *)Actor_FilesLoaded,
    (void *)Actor_Unload, (void *)Actor_RememberPos, (void *)Actor_Place, (void *)Thing08_Draw,
    (void *)Thing08_Frame, (void *)Actor_LeaveScreen, (void *)Actor_ShowUp, (void *)Actor_CanAct,
    (void *)Thing_PutAt, (void *)ThingShared_PutDown, (void *)ThingShared_LandTri, (void *)ThingShared_MotionState,
    (void *)ThingShared_Flying, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Thing04_dtor, (void *)Thing04_Setup,
    (void *)Actor_Cleanup, (void *)Actor_LoadFiles, (void *)Actor_FilesLoading, (void *)Actor_FilesLoaded,
    (void *)Actor_Unload, (void *)Actor_RememberPos, (void *)Actor_Place, (void *)Thing04_Draw,
    (void *)Thing04_Frame, (void *)Actor_LeaveScreen, (void *)Actor_ShowUp, (void *)Actor_CanAct,
    (void *)Thing_PutAt, (void *)Thing04_PutDown, (void *)Thing04_LandTri, (void *)Thing04_MotionState,
    (void *)Thing04_Flying, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Costume2Model_dtor, (void *)Costume2Model_Loaded,
    (void *)func_00337CE0, (void *)HumanModel_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)Model_Vt2C,
    (void *)Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)Costume2Model_Springs,
    (void *)HumanModel_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)HumanModel_PlantFeet,
    (void *)Model_FeetReplace, (void *)HumanModel_FeetUp, (void *)HumanModel_Vt58, (void *)Model_EaseTilt,
    (void *)HumanModel_HeadPos, (void *)Model_FootDown, (void *)CharModel_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)HumanModel_Vt74, (void *)HumanModel_Vt78, (void *)HumanModel_Vt7C,
    (void *)HumanModel_HeadBone, (void *)Costume2Model_Part0, (void *)Costume2Model_Part1, (void *)Costume2Model_Part2,
    (void *)Costume2Model_Part3, (void *)Costume2Model_Part4, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)Costume2Model_ModelFile, (void *)CharModel_MarkerFile, (void *)Costume2Model_Textures, (void *)Costume2Model_Textures2,
    (void *)Costume2Model_BufferSize, (void *)CharModel_SecondaryMotion, (void *)Costume2Model_MotionTable, (void *)CharModel_SetFlag,
    (void *)Costume2Model_SetPoint, (void *)Costume2Model_ShowParts, (void *)CharModel_SetVector, (void *)Costume2Model_VtCC,
    (void *)Costume2Model_VtD0, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Costume3Model_dtor, (void *)Costume3Model_Loaded,
    (void *)func_00338AE0, (void *)HumanModel_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)Model_Vt2C,
    (void *)Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)Costume3Model_Springs,
    (void *)HumanModel_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)HumanModel_PlantFeet,
    (void *)Model_FeetReplace, (void *)HumanModel_FeetUp, (void *)HumanModel_Vt58, (void *)Model_EaseTilt,
    (void *)HumanModel_HeadPos, (void *)Model_FootDown, (void *)CharModel_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)HumanModel_Vt74, (void *)HumanModel_Vt78, (void *)HumanModel_Vt7C,
    (void *)HumanModel_HeadBone, (void *)Costume3Model_Part0, (void *)Costume3Model_Part1, (void *)Costume3Model_Part2,
    (void *)Costume3Model_Part3, (void *)Costume3Model_Part4, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)Costume3Model_ModelFile, (void *)CharModel_MarkerFile, (void *)Costume3Model_Textures, (void *)Costume3Model_Textures2,
    (void *)Costume3Model_BufferSize, (void *)CharModel_SecondaryMotion, (void *)Costume3Model_MotionTable, (void *)CharModel_SetFlag,
    (void *)Costume3Model_SetPoint, (void *)CharModel_ShowParts, (void *)CharModel_SetVector, (void *)Costume3Model_VtCC,
    (void *)CharModel_VtD0, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SpringPart_Reset, (void *)SpringPart_NoopC,
    (void *)CostumeHangPoint_Frame, (void *)SpringPartBase_AdjustBone, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item15_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item15_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item15_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item16_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item16_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item16_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item17_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item17_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item17_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind28_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind28_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind28_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)func_00339B40, (void *)Kind28_ModelFiles, (void *)Kind28_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind28_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomC7_dtor, (void *)RoomC7_EnterScript,
    (void *)RoomC7_Phase1Script, (void *)RoomC7_Phase2Script, (void *)RoomC7_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomC7_Phase5Script, (void *)RoomC7_ActionScript, (void *)RoomC7_Command, (void *)RoomC7_Condition,
    (void *)RoomC7_CharEnterScript, (void *)RoomC7_ObjectName, (void *)RoomC7_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind29_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind29_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind29_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)func_0033ACD0, (void *)Kind29_ModelFiles, (void *)Kind29_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind29_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemA2_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD640_Kind, (void *)ItemClassD640_Actions, (void *)ItemClassD640_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)ItemA2_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemA2_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind30_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind30_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind30_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)func_0033B050, (void *)Kind30_ModelFiles, (void *)Kind30_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind30_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind31_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind31_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind31_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)func_0033B320, (void *)Kind31_ModelFiles, (void *)Kind31_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind31_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemClass6B00_dtor, (void *)PoolEntry_Id,
    (void *)ItemClass6B00_Kind, (void *)ItemClass6B00_Actions, (void *)ItemClass6B00_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)PoolEntry_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)PoolEntry_Use,
    (void *)ItemClass6B00_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)FixModelDraw_dtor, (void *)FixModel_Draw,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ItemA3_dtor, (void *)PoolEntry_Id,
    (void *)ItemClassD640_Kind, (void *)ItemClassD640_Actions, (void *)ItemClassD640_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)ItemA3_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)ItemA3_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)DustShaft_dtor, (void *)DustShaft_Start,
    (void *)DustShaft_Update, (void *)DustShaft_Draw, (void *)EffectBase_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SpiralSmoke_dtor, (void *)SpiralSmoke_Start,
    (void *)SpiralSmoke_Update, (void *)SpiralSmoke_Draw, (void *)EffectBase_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Drips_dtor, (void *)Drips_Start,
    (void *)Drips_Update, (void *)Drips_Draw, (void *)EffectBase_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind32_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind32_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind32_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)func_0033D7F0, (void *)Kind32_ModelFiles, (void *)Kind32_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind32_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)LoadingEmblem_dtor, (void *)LoadingEmblem_Draw,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind18Model_dtor, (void *)Kind18Model_Loaded,
    (void *)Kind18Model_Frame, (void *)HumanModel_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)Model_Vt2C,
    (void *)Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)EventHumanModel_Springs,
    (void *)HumanModel_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)HumanModel_PlantFeet,
    (void *)Model_FeetReplace, (void *)HumanModel_FeetUp, (void *)HumanModel_Vt58, (void *)Model_EaseTilt,
    (void *)HumanModel_HeadPos, (void *)Model_FootDown, (void *)CharModel_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)HumanModel_Vt74, (void *)HumanModel_Vt78, (void *)HumanModel_Vt7C,
    (void *)HumanModel_HeadBone, (void *)EventHumanModel_Part0, (void *)EventHumanModel_Part1, (void *)EventHumanModel_Part2,
    (void *)EventHumanModel_Part3, (void *)EventHumanModel_Part4, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)EventHumanModel_ModelFile, (void *)CharModel_MarkerFile, (void *)EventHumanModel_Textures, (void *)EventHumanModel_Textures2,
    (void *)EventHumanModel_BufferSize, (void *)CharModel_SecondaryMotion, (void *)EventHumanModel_MotionTable, (void *)EventHumanModel_SetFlag,
    (void *)EventHumanModel_SetPoint, (void *)Kind18Model_ShowParts, (void *)CharModel_SetVector, (void *)CharModel_VtCC,
    (void *)CharModel_VtD0, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item18_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)Item18_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item18_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item18_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item19_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)Item19_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item19_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item19_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item1A_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)Item1A_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item1A_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item1A_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item1B_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)Item1B_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item1B_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item1B_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item1C_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)Item1C_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item1C_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item1C_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room81_dtor, (void *)Room81_EnterScript,
    (void *)Room81_Phase1Script, (void *)Room81_Phase2Script, (void *)Room81_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room81_Phase5Script, (void *)Room81_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room81_CharEnterScript, (void *)Room81_ObjectName, (void *)Room81_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room83_dtor, (void *)Room83_EnterScript,
    (void *)Room83_Phase1Script, (void *)Room83_Phase2Script, (void *)Room83_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room83_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room83_CharEnterScript, (void *)Room83_ObjectName, (void *)Room83_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room84_dtor, (void *)Room84_EnterScript,
    (void *)Room84_Phase1Script, (void *)Room84_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room84_ActionScript, (void *)Room84_Command, (void *)RoomBase_Condition,
    (void *)Room84_CharEnterScript, (void *)Room84_ObjectName, (void *)Room84_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room85_dtor, (void *)Room85_EnterScript,
    (void *)Room85_Phase1Script, (void *)Room85_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room85_ActionScript, (void *)Room85_Command, (void *)RoomBase_Condition,
    (void *)Room85_CharEnterScript, (void *)Room85_ObjectName, (void *)Room85_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room87_dtor, (void *)Room87_EnterScript,
    (void *)Room87_Phase1Script, (void *)Room87_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room87_Phase5Script, (void *)Room87_ActionScript, (void *)Room87_Command, (void *)RoomBase_Condition,
    (void *)Room87_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room88_dtor, (void *)Room88_EnterScript,
    (void *)Room88_Phase1Script, (void *)Room88_Phase2Script, (void *)Room88_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room88_ActionScript, (void *)Room88_Command, (void *)RoomBase_Condition,
    (void *)Room88_CharEnterScript, (void *)Room88_ObjectName, (void *)Room88_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room8A_dtor, (void *)Room8A_EnterScript,
    (void *)Room8A_Phase1Script, (void *)Room8A_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room8A_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room8A_CharEnterScript, (void *)Room8A_ObjectName, (void *)Room8A_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room8C_dtor, (void *)Room8C_EnterScript,
    (void *)Room8C_Phase1Script, (void *)Room8C_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room8C_Phase5Script, (void *)Room8C_ActionScript, (void *)Room8C_Command, (void *)RoomBase_Condition,
    (void *)Room8C_CharEnterScript, (void *)Room8C_ObjectName, (void *)Room8C_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room8D_dtor, (void *)Room8D_EnterScript,
    (void *)Room8D_Phase1Script, (void *)Room8D_Phase2Script, (void *)Room8D_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room8D_Phase5Script, (void *)Room8D_ActionScript, (void *)Room8D_Command, (void *)RoomBase_Condition,
    (void *)Room8D_CharEnterScript, (void *)Room8D_ObjectName, (void *)Room8D_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room8E_dtor, (void *)Room8E_EnterScript,
    (void *)Room8E_Phase1Script, (void *)Room8E_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room8E_ActionScript, (void *)Room8E_Command, (void *)RoomBase_Condition,
    (void *)Room8E_CharEnterScript, (void *)Room8E_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room8F_dtor, (void *)Room8F_EnterScript,
    (void *)Room8F_Phase1Script, (void *)Room8F_Phase2Script, (void *)Room8F_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room8F_ActionScript, (void *)Room8F_Command, (void *)Room8F_Condition,
    (void *)Room8F_CharEnterScript, (void *)Room8F_ObjectName, (void *)Room8F_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room91_dtor, (void *)Room91_EnterScript,
    (void *)Room91_Phase1Script, (void *)Room91_Phase2Script, (void *)Room91_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room91_ActionScript, (void *)Room91_Command, (void *)RoomBase_Condition,
    (void *)Room91_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room91_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room92_dtor, (void *)Room92_EnterScript,
    (void *)Room92_Phase1Script, (void *)Room92_Phase2Script, (void *)Room92_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room92_ActionScript, (void *)Room92_Command, (void *)RoomBase_Condition,
    (void *)Room92_CharEnterScript, (void *)Room92_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room97_dtor, (void *)Room97_EnterScript,
    (void *)Room97_Phase1Script, (void *)Room97_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room97_ActionScript, (void *)Room97_Command, (void *)RoomBase_Condition,
    (void *)Room97_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room97_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room69_dtor, (void *)Room69_EnterScript,
    (void *)Room69_Phase1Script, (void *)Room69_Phase2Script, (void *)Room69_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room69_Phase5Script, (void *)Room69_ActionScript, (void *)Room69_Command, (void *)RoomBase_Condition,
    (void *)Room69_CharEnterScript, (void *)Room69_ObjectName, (void *)Room69_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room6A_dtor, (void *)Room6A_EnterScript,
    (void *)Room6A_Phase1Script, (void *)Room6A_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room6A_Phase5Script, (void *)Room6A_ActionScript, (void *)Room6A_Command, (void *)RoomBase_Condition,
    (void *)Room6A_CharEnterScript, (void *)Room6A_ObjectName, (void *)Room6A_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item1D_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)Item1D_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item1D_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item1D_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room6B_dtor, (void *)Room6B_EnterScript,
    (void *)Room6B_Phase1Script, (void *)Room6B_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room6B_Phase5Script, (void *)Room6B_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room6B_CharEnterScript, (void *)Room6B_ObjectName, (void *)Room6B_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room6C_dtor, (void *)Room6C_EnterScript,
    (void *)Room6C_Phase1Script, (void *)Room6C_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room6C_Phase5Script, (void *)Room6C_ActionScript, (void *)RoomBase_Command, (void *)Room6C_Condition,
    (void *)Room6C_CharEnterScript, (void *)Room6C_ObjectName, (void *)Room6C_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room6D_dtor, (void *)Room6D_EnterScript,
    (void *)Room6D_Phase1Script, (void *)Room6D_Phase2Script, (void *)Room6D_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room6D_ActionScript, (void *)RoomBase_Command, (void *)Room6D_Condition,
    (void *)Room6D_CharEnterScript, (void *)Room6D_ObjectName, (void *)Room6D_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room6E_dtor, (void *)Room6E_EnterScript,
    (void *)Room6E_Phase1Script, (void *)Room6E_Phase2Script, (void *)Room6E_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room6E_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room6E_CharEnterScript, (void *)Room6E_ObjectName, (void *)Room6E_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room6F_dtor, (void *)Room6F_EnterScript,
    (void *)Room6F_Phase1Script, (void *)Room6F_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room6F_Phase5Script, (void *)Room6F_ActionScript, (void *)Room6F_Command, (void *)Room6F_Condition,
    (void *)Room6F_CharEnterScript, (void *)Room6F_ObjectName, (void *)Room6F_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room70_dtor, (void *)Room70_EnterScript,
    (void *)Room70_Phase1Script, (void *)Room70_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room70_Phase5Script, (void *)Room70_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room70_CharEnterScript, (void *)Room70_ObjectName, (void *)Room70_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind33_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind33_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind33_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)func_00345230, (void *)Kind33_ModelFiles, (void *)Kind33_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind33_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room66Effect_dtor, (void *)Room66Effect_Start,
    (void *)Room66Effect_Update, (void *)Room66Effect_Draw, (void *)Room66Effect_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind34_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Kind34_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Kind34_Update, (void *)Pursuer_LeaveScreen, (void *)Pursuer_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Kind34_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Pursuer_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Kind34_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)Kind34_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)Kind34_ExitArg, (void *)Kind34_Setup, (void *)Kind34_ModelFiles, (void *)Kind34_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Pursuer_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Kind34_AttackTable, (void *)Pursuer_PickAttack, (void *)Kind34_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Kind34_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Kind34_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Kind34_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Kind34_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Kind34_ThreatAmount, (void *)Kind34_AttackAnimA,
    (void *)Kind34_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Kind34_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)Kind34_Kind,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room55Effect_dtor, (void *)Room55Effect_Start,
    (void *)Room55Effect_Update, (void *)Room55Effect_Draw, (void *)Room55Effect_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind35_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Kind35_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Kind35_Update, (void *)Pursuer_LeaveScreen, (void *)Pursuer_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Pursuer_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Kind35_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)Kind35_Setup, (void *)Kind35_ModelFiles, (void *)Kind35_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Pursuer_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Kind35_AttackTable, (void *)Pursuer_PickAttack, (void *)Kind35_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Kind35_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Kind35_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Kind35_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Kind35_ThreatAmount, (void *)Kind35_AttackAnimA,
    (void *)Kind35_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)Kind35_Kind,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind36_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Kind36_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Kind36_Update, (void *)Pursuer_LeaveScreen, (void *)Pursuer_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Pursuer_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Kind36_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)Kind36_Setup, (void *)Kind36_ModelFiles, (void *)Kind36_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Pursuer_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Kind36_AttackTable, (void *)Pursuer_PickAttack, (void *)Kind36_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Kind36_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Kind36_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Kind36_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Kind36_ThreatAmount, (void *)Kind36_AttackAnimA,
    (void *)Kind36_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)Kind36_Kind,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Costume6Model_dtor, (void *)Costume6Model_Loaded,
    (void *)func_00349FE0, (void *)HumanModel_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)Costume6Model_Vt2C,
    (void *)Costume6Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)Costume6Model_Springs,
    (void *)HumanModel_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)HumanModel_PlantFeet,
    (void *)Model_FeetReplace, (void *)HumanModel_FeetUp, (void *)HumanModel_Vt58, (void *)Model_EaseTilt,
    (void *)HumanModel_HeadPos, (void *)Model_FootDown, (void *)CharModel_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)HumanModel_Vt74, (void *)HumanModel_Vt78, (void *)HumanModel_Vt7C,
    (void *)HumanModel_HeadBone, (void *)Costume6Model_Part0, (void *)Costume6Model_Part1, (void *)Costume6Model_Part2,
    (void *)Costume6Model_Part3, (void *)Costume6Model_Part4, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)Costume6Model_ModelFile, (void *)Costume6Model_MarkerFile, (void *)Costume6Model_Textures, (void *)Costume6Model_Textures2,
    (void *)Costume6Model_BufferSize, (void *)CharModel_SecondaryMotion, (void *)Costume6Model_MotionTable, (void *)Costume6Model_SetFlag,
    (void *)Costume6Model_SetPoint, (void *)CharModel_ShowParts, (void *)CharModel_SetVector, (void *)CharModel_VtCC,
    (void *)CharModel_VtD0, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room80_dtor, (void *)Room80_EnterScript,
    (void *)Room80_Phase1Script, (void *)Room80_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room80_ActionScript, (void *)Room80_Command, (void *)RoomBase_Condition,
    (void *)Room80_CharEnterScript, (void *)Room80_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomC1_dtor, (void *)RoomC1_EnterScript,
    (void *)RoomC1_Phase1Script, (void *)RoomC1_Phase2Script, (void *)RoomC1_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomC1_ActionScript, (void *)RoomBase_Command, (void *)RoomC1_Condition,
    (void *)RoomC1_CharEnterScript, (void *)RoomC1_ObjectName, (void *)RoomC1_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomC2_dtor, (void *)RoomC2_EnterScript,
    (void *)RoomC2_Phase1Script, (void *)RoomC2_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomC2_Phase5Script, (void *)RoomC2_ActionScript, (void *)RoomC2_Command, (void *)RoomC2_Condition,
    (void *)RoomC2_CharEnterScript, (void *)RoomC2_ObjectName, (void *)RoomC2_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomC3_dtor, (void *)RoomC3_EnterScript,
    (void *)RoomC3_Phase1Script, (void *)RoomC3_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomC3_Phase5Script, (void *)RoomC3_ActionScript, (void *)RoomC3_Command, (void *)RoomC3_Condition,
    (void *)RoomC3_CharEnterScript, (void *)RoomC3_ObjectName, (void *)RoomC3_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomC4_dtor, (void *)RoomC4_EnterScript,
    (void *)RoomC4_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomC4_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)RoomC4_CharEnterScript, (void *)RoomC4_ObjectName, (void *)RoomC4_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomC5_dtor, (void *)RoomC5_EnterScript,
    (void *)RoomC5_Phase1Script, (void *)RoomC5_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomC5_Phase5Script, (void *)RoomC5_ActionScript, (void *)RoomC5_Command, (void *)RoomBase_Condition,
    (void *)RoomC5_CharEnterScript, (void *)RoomC5_ObjectName, (void *)RoomC5_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomC6_dtor, (void *)RoomC6_EnterScript,
    (void *)RoomC6_Phase1Script, (void *)RoomC6_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomC6_Phase5Script, (void *)RoomC6_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)RoomC6_CharEnterScript, (void *)RoomC6_ObjectName, (void *)RoomC6_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomC8_dtor, (void *)RoomC8_EnterScript,
    (void *)RoomC8_Phase1Script, (void *)RoomC8_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomC8_ActionScript, (void *)RoomC8_Command, (void *)RoomBase_Condition,
    (void *)RoomC8_CharEnterScript, (void *)RoomC8_ObjectName, (void *)RoomC8_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind37_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Kind37_LightChange,
    (void *)Kind37_Update, (void *)Pursuer_LeaveScreen, (void *)Pursuer_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Pursuer_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Kind37_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)Kind37_Setup, (void *)Kind37_ModelFiles, (void *)Kind37_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Pursuer_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Kind37_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Kind37_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Kind37_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Kind37_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Kind37_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Kind37_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Kind37_FrightSeen, (void *)Kind37_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Kind37_AttackAnimA,
    (void *)Kind37_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Kind37_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room34_dtor, (void *)Room34_EnterScript,
    (void *)Room34_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room34_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room34_CharEnterScript, (void *)Room34_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MusicStage3_dtor, (void *)MusicStage3_Load,
    (void *)MusicDir_BanksIn, (void *)MusicDir_Reset, (void *)MusicDir_Release, (void *)MusicDir_Hold,
    (void *)MusicDir_SetMusicVolume, (void *)MusicDir_SetProgressVolume, (void *)MusicDir_Volume4, (void *)MusicDir_Get12,
    (void *)MusicDir_VolumeC, (void *)MusicDir_Get14, (void *)MusicDir_GlobalVolumeTo, (void *)MusicDir_Start,
    (void *)MusicStage3_StageChans, (void *)MusicDir_SetVolume, (void *)MusicDir_FullVolume, (void *)MusicDir_Silence,
    (void *)MusicDir_StateFrame, (void *)MusicDir_ChaseBegins, (void *)MusicStage3_Lost, (void *)MusicStage3_Seen,
    (void *)MusicDir_Panic, (void *)MusicDir_PanicOver, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Fire_dtor, (void *)Fire_Start,
    (void *)Fire_Update, (void *)Fire_Draw, (void *)Fire_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Caustic_dtor, (void *)Caustic_Draw,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room33_dtor, (void *)Room33_EnterScript,
    (void *)Room33_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room33_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room33_CharEnterScript, (void *)Room33_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Effect78BC0_dtor, (void *)Effect78BC0_Start,
    (void *)Effect78BC0_Update, (void *)Effect78BC0_Draw, (void *)Effect78BC0_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room4EEffect_dtor, (void *)Room4EEffect_Start,
    (void *)Room4EEffect_Update, (void *)Room4EEffect_Draw, (void *)EffectBase_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room98_dtor, (void *)Room98_EnterScript,
    (void *)Room98_Phase1Script, (void *)Room98_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room98_ActionScript, (void *)Room98_Command, (void *)RoomBase_Condition,
    (void *)Room98_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room98_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room35_dtor, (void *)Room35_EnterScript,
    (void *)RoomBase_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room35_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room35_CharEnterScript, (void *)Room35_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item1E_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item1E_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item1E_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item1F_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item1F_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item1F_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item20_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item20_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item20_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item21_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item21_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item21_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item22_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item22_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item22_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item23_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item23_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item23_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item24_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item24_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item24_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item25_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item25_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item25_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item26_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item26_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item26_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item27_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item27_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item27_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item28_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item28_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item28_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind38_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Pursuer_Update, (void *)Pursuer_LeaveScreen, (void *)Kind38_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Pursuer_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Kind38_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)func_00352050, (void *)Kind38_ModelFiles, (void *)Kind38_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Kind38_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Pursuer_AttackTable, (void *)Pursuer_PickAttack, (void *)Pursuer_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Pursuer_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Pursuer_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Pursuer_ReachHewie, (void *)Pursuer_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Pursuer_FrightSeen, (void *)Pursuer_FrightAttack, (void *)Pursuer_ThreatAmount, (void *)Pursuer_AttackAnimA,
    (void *)Pursuer_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Wisps_dtor, (void *)Wisps_Start,
    (void *)Wisps_Update, (void *)Wisps_Draw, (void *)Wisps_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room99_dtor, (void *)Room99_EnterScript,
    (void *)Room99_Phase1Script, (void *)Room99_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room99_Phase5Script, (void *)Room99_ActionScript, (void *)Room99_Command, (void *)RoomBase_Condition,
    (void *)Room99_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room99_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room9A_dtor, (void *)Room9A_EnterScript,
    (void *)Room9A_Phase1Script, (void *)Room9A_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room9A_ActionScript, (void *)Room9A_Command, (void *)RoomBase_Condition,
    (void *)Room9A_CharEnterScript, (void *)RoomBase_ObjectName, (void *)Room9A_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room9B_dtor, (void *)Room9B_EnterScript,
    (void *)Room9B_Phase1Script, (void *)Room9B_Phase2Script, (void *)Room9B_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room9B_ActionScript, (void *)Room9B_Command, (void *)RoomBase_Condition,
    (void *)Room9B_CharEnterScript, (void *)Room9B_ObjectName, (void *)Room9B_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)FallingDrops_dtor, (void *)FallingDrops_Start,
    (void *)FallingDrops_Update, (void *)FallingDrops_Draw, (void *)EffectBase_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Costume7Model_dtor, (void *)Costume7Model_Loaded,
    (void *)func_003543B0, (void *)HumanModel_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)Costume7Model_Vt2C,
    (void *)Costume7Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)Costume7Model_Springs,
    (void *)HumanModel_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)HumanModel_PlantFeet,
    (void *)Model_FeetReplace, (void *)HumanModel_FeetUp, (void *)HumanModel_Vt58, (void *)Model_EaseTilt,
    (void *)HumanModel_HeadPos, (void *)Model_FootDown, (void *)CharModel_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)HumanModel_Vt74, (void *)HumanModel_Vt78, (void *)HumanModel_Vt7C,
    (void *)HumanModel_HeadBone, (void *)Costume7Model_Part0, (void *)Costume7Model_Part1, (void *)Costume7Model_Part2,
    (void *)Costume7Model_Part3, (void *)Costume7Model_Part4, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)Costume7Model_ModelFile, (void *)Costume7Model_MarkerFile, (void *)Costume7Model_Textures, (void *)Costume7Model_Textures2,
    (void *)Costume7Model_BufferSize, (void *)CharModel_SecondaryMotion, (void *)Costume7Model_MotionTable, (void *)CharModel_SetFlag,
    (void *)Costume7Model_SetPoint, (void *)CharModel_ShowParts, (void *)CharModel_SetVector, (void *)CharModel_VtCC,
    (void *)CharModel_VtD0, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ThingShared_dtor, (void *)ThingShared_Setup,
    (void *)Actor_Cleanup, (void *)Actor_LoadFiles, (void *)Actor_FilesLoading, (void *)Actor_FilesLoaded,
    (void *)Actor_Unload, (void *)Actor_RememberPos, (void *)Actor_Place, (void *)ThingShared_Draw,
    (void *)ThingShared_Frame, (void *)Actor_LeaveScreen, (void *)Actor_ShowUp, (void *)Actor_CanAct,
    (void *)Thing_PutAt, (void *)ThingShared_PutDown, (void *)ThingShared_LandTri, (void *)ThingShared_MotionState,
    (void *)ThingShared_Flying, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MirrorFragment_dtor, (void *)MirrorFragment_Start,
    (void *)MirrorFragment_Update, (void *)MirrorFragment_Draw, (void *)MirrorFragment_SetParams, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)BigFire_dtor, (void *)BigFire_Start,
    (void *)BigFire_Update, (void *)BigFire_Draw, (void *)BigFire_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)FloorGlow_dtor, (void *)FloorGlow_Start,
    (void *)FloorGlow_Update, (void *)FloorGlow_Draw, (void *)FloorGlow_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room60Effect_dtor, (void *)Room60Effect_Start,
    (void *)Room60Effect_Update, (void *)Room60Effect_Draw, (void *)Room60Effect_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Cr19Bubbles_dtor, (void *)Cr19Bubbles_Start,
    (void *)Cr19Bubbles_Update, (void *)Cr19Bubbles_Draw, (void *)Cr19Bubbles_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)StrikeMark_dtor, (void *)StrikeMark_Start,
    (void *)StrikeMark_Update, (void *)StrikeMark_Draw, (void *)StrikeMark_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room36_dtor, (void *)Room36_EnterScript,
    (void *)RoomBase_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room36_ActionScript, (void *)RoomBase_Command, (void *)RoomBase_Condition,
    (void *)Room36_CharEnterScript, (void *)Room36_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Costume8Model_dtor, (void *)Costume8Model_Loaded,
    (void *)func_0035B110, (void *)HumanModel_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)Model_Vt2C,
    (void *)Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)HumanModel_Vt3C,
    (void *)HumanModel_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)HumanModel_PlantFeet,
    (void *)Model_FeetReplace, (void *)HumanModel_FeetUp, (void *)HumanModel_Vt58, (void *)Model_EaseTilt,
    (void *)HumanModel_HeadPos, (void *)Model_FootDown, (void *)CharModel_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)HumanModel_Vt74, (void *)HumanModel_Vt78, (void *)HumanModel_Vt7C,
    (void *)HumanModel_HeadBone, (void *)Costume8Model_Part0, (void *)Costume8Model_Part1, (void *)Costume8Model_Part2,
    (void *)Costume8Model_Part3, (void *)Costume8Model_Part4, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)Costume8Model_ModelFile, (void *)CharModel_MarkerFile, (void *)Costume8Model_Textures, (void *)Costume8Model_Textures2,
    (void *)Costume8Model_BufferSize, (void *)CharModel_SecondaryMotion, (void *)Costume8Model_MotionTable, (void *)CharModel_SetFlag,
    (void *)CharModel_SetPoint, (void *)CharModel_ShowParts, (void *)CharModel_SetVector, (void *)CharModel_VtCC,
    (void *)CharModel_VtD0, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind12Model_dtor, (void *)Kind12Model_Loaded,
    (void *)func_0035B310, (void *)HumanModel_AdjustBone, (void *)Model_FeetContact, (void *)Model_Vt1C,
    (void *)Model_Vt20, (void *)Model_Vt24, (void *)Model_SetMatrix, (void *)Model_Vt2C,
    (void *)Model_Vt30, (void *)Model_Set878, (void *)Model_Draw, (void *)HumanModel_Vt3C,
    (void *)HumanModel_BodyFrames, (void *)Model_SlopeKeep, (void *)Model_FloorLevel, (void *)HumanModel_PlantFeet,
    (void *)Model_FeetReplace, (void *)HumanModel_FeetUp, (void *)HumanModel_Vt58, (void *)Model_EaseTilt,
    (void *)HumanModel_HeadPos, (void *)Model_FootDown, (void *)Model_ClearDraw, (void *)Model_HalfHip,
    (void *)Model_Vt70, (void *)HumanModel_Vt74, (void *)HumanModel_Vt78, (void *)HumanModel_Vt7C,
    (void *)HumanModel_HeadBone, (void *)Kind12Model_Part0, (void *)Kind12Model_Part1, (void *)Kind12Model_Part2,
    (void *)Kind12Model_Part3, (void *)Model_Vt94, (void *)Model_Vt98, (void *)Model_Vt9C,
    (void *)Model_ModelFile, (void *)Model_MarkerFile, (void *)Model_Textures, (void *)Model_VtAC,
    (void *)Model_VtB0, (void *)Kind12Model_SecondaryMotion, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)BoneSmoke_dtor, (void *)BoneSmoke_Start,
    (void *)BoneSmoke_Update, (void *)BoneSmoke_Draw, (void *)BoneSmoke_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Item29_dtor, (void *)PoolEntry_Id,
    (void *)PlainItem_Kind, (void *)PlainItem_Actions, (void *)PlainItem_IsCounted, (void *)PoolEntry_SortKey,
    (void *)PoolEntry_Get20, (void *)PoolEntry_Data, (void *)Item29_LoadPicture, (void *)PoolEntry_AddStack,
    (void *)PoolEntry_TakeOne, (void *)PoolEntry_Count, (void *)PoolEntry_CountUp, (void *)Item29_Use,
    (void *)PoolEntry_PartnerCheck, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ObjectGlow_dtor, (void *)ObjectGlow_Start,
    (void *)ObjectGlow_Update, (void *)ObjectGlow_Draw, (void *)ObjectGlow_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)WallShadow_dtor, (void *)WallShadow_Start,
    (void *)WallShadow_Update, (void *)WallShadow_Draw, (void *)WallShadow_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomC7Cursor_dtor, (void *)RoomC7Cursor_Start,
    (void *)RoomC7Cursor_Update, (void *)RoomC7Cursor_Draw, (void *)RoomC7Cursor_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room93_dtor, (void *)Room93_EnterScript,
    (void *)Room93_Phase1Script, (void *)Room93_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room93_Phase5Script, (void *)Room93_ActionScript, (void *)Room93_Command, (void *)RoomBase_Condition,
    (void *)Room93_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room94_dtor, (void *)Room94_EnterScript,
    (void *)Room94_Phase1Script, (void *)Room94_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room94_Phase5Script, (void *)Room94_ActionScript, (void *)Room94_Command, (void *)RoomBase_Condition,
    (void *)Room94_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room95_dtor, (void *)Room95_EnterScript,
    (void *)Room95_Phase1Script, (void *)Room95_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room95_Phase5Script, (void *)Room95_ActionScript, (void *)Room95_Command, (void *)RoomBase_Condition,
    (void *)Room95_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room96_dtor, (void *)Room96_EnterScript,
    (void *)Room96_Phase1Script, (void *)Room96_Phase2Script, (void *)RoomBase_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)Room96_Phase5Script, (void *)Room96_ActionScript, (void *)Room96_Command, (void *)RoomBase_Condition,
    (void *)Room96_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Debris_dtor, (void *)Debris_Start,
    (void *)Debris_Update, (void *)Debris_Draw, (void *)Debris_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)MusicStage4_dtor, (void *)MusicStage4_Load,
    (void *)MusicDir_BanksIn, (void *)MusicDir_Reset, (void *)MusicDir_Release, (void *)MusicDir_Hold,
    (void *)MusicDir_SetMusicVolume, (void *)MusicDir_SetProgressVolume, (void *)MusicDir_Volume4, (void *)MusicDir_Get12,
    (void *)MusicDir_VolumeC, (void *)MusicDir_Get14, (void *)MusicDir_GlobalVolumeTo, (void *)MusicDir_Start,
    (void *)MusicStage4_StageChans, (void *)MusicDir_SetVolume, (void *)MusicDir_FullVolume, (void *)MusicDir_Silence,
    (void *)MusicDir_StateFrame, (void *)MusicDir_ChaseBegins, (void *)MusicStage4_Lost, (void *)MusicStage4_Seen,
    (void *)MusicDir_Panic, (void *)MusicDir_PanicOver, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)CeilingDrips_dtor, (void *)CeilingDrips_Start,
    (void *)CeilingDrips_Update, (void *)CeilingDrips_Draw, (void *)EffectBase_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)OneDrip_dtor, (void *)OneDrip_Start,
    (void *)OneDrip_Update, (void *)OneDrip_Draw, (void *)EffectBase_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)DropletSpray_dtor, (void *)DropletSpray_Start,
    (void *)DropletSpray_Update, (void *)DropletSpray_Draw, (void *)DropletSpray_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)WindowFlash_dtor, (void *)WindowFlash_Start,
    (void *)WindowFlash_Update, (void *)WindowFlash_Draw, (void *)WindowFlash_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SplashRing_dtor, (void *)SplashRing_Start,
    (void *)SplashRing_Update, (void *)SplashRing_Draw, (void *)SplashRing_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Effect79B00_dtor, (void *)Effect79B00_Start,
    (void *)Effect79B00_Update, (void *)Effect79B00_Draw, (void *)Effect79B00_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Kind39_dtor, (void *)Pursuer_Reset,
    (void *)Pursuer_Cleanup, (void *)Pursuer_LoadFiles, (void *)Pursuer_FilesLoading, (void *)Pursuer_FilesLoaded,
    (void *)Pursuer_Unload, (void *)Pursuer_RememberPos, (void *)Pursuer_Place, (void *)Pursuer_LightChange,
    (void *)Kind39_Update, (void *)Pursuer_LeaveScreen, (void *)Pursuer_ShowUp, (void *)Actor_CanAct,
    (void *)Pursuer_ModelUpdate, (void *)Pursuer_Think, (void *)Pursuer_PlaceModel, (void *)Pursuer_Disable,
    (void *)Pursuer_Enable, (void *)Pursuer_LoadMessage, (void *)Pursuer_Deactivate, (void *)Kind39_Activate,
    (void *)Pursuer_ResetBehaviour, (void *)Pursuer_PlaceInRoom, (void *)Pursuer_EventConcerns, (void *)Pursuer_SaveState,
    (void *)Pursuer_LoadState, (void *)Pursuer_AttackPoint, (void *)Pursuer_LeftBehind, (void *)Pursuer_BackToStance,
    (void *)Character_RegionFade, (void *)Pursuer_EventState, (void *)Pursuer_EventCommand, (void *)Pursuer_BackToNormal,
    (void *)Pursuer_EventReset, (void *)Character_TimerDown, (void *)Pursuer_IsBusy, (void *)Pursuer_DoorOffset,
    (void *)Pursuer_TurnRate, (void *)Pursuer_TurnRateFast, (void *)NPC_BlockFlags, (void *)NPC_GoTo,
    (void *)NPC_HeadForFiona, (void *)NPC_HeadFor, (void *)NPC_HeadNearFiona, (void *)NPC_HeadNearHewie,
    (void *)NPC_FionaInReach, (void *)NPC_HewieInReach, (void *)NPC_HearNoise, (void *)Pursuer_PickTarget,
    (void *)Pursuer_LocateTarget, (void *)NPC_PathLengthTo, (void *)NPC_PathLengthGoal, (void *)NPC_PathLengthSpot,
    (void *)NPC_PathLengthChar, (void *)NPC_DoorBreak, (void *)NPC_PickDestination, (void *)NPC_CanUseExit,
    (void *)NPC_ExitArg, (void *)Kind39_Setup, (void *)Kind39_ModelFiles, (void *)Kind39_MotionFiles,
    (void *)Pursuer_Footsteps, (void *)Pursuer_StopSounds, (void *)Pursuer_PathNodeSound, (void *)Pursuer_InStance2Anim,
    (void *)Pursuer_GrabOrder, (void *)Pursuer_StartAction, (void *)Pursuer_StartActionNext, (void *)Pursuer_GivesUp,
    (void *)Pursuer_ModesSearching, (void *)Pursuer_Modes, (void *)Pursuer_StandAnim, (void *)Pursuer_Threat,
    (void *)Kind39_AttackTable, (void *)Pursuer_PickAttack, (void *)Kind39_BonePositions, (void *)Pursuer_PickStep,
    (void *)Pursuer_PickStepAlt, (void *)Pursuer_LeaveScreenEnding, (void *)Pursuer_EnterRoom, (void *)Pursuer_EventOver,
    (void *)Pursuer_EventOver2, (void *)Pursuer_AfterMove, (void *)Pursuer_PlanWayOn, (void *)Pursuer_PlanToGoal,
    (void *)Pursuer_FollowPlan, (void *)Pursuer_Pace, (void *)Pursuer_TravelOffscreen, (void *)Pursuer_Plan16C,
    (void *)Pursuer_Plan170, (void *)Pursuer_KnockAtDoor, (void *)Pursuer_WaitRoomFlag, (void *)Pursuer_Move17C,
    (void *)Pursuer_Move180, (void *)Pursuer_Move184, (void *)Pursuer_BackToStand, (void *)Pursuer_Move18C,
    (void *)Pursuer_Nothing190, (void *)Pursuer_WalkToGoal, (void *)Pursuer_StepBack, (void *)Pursuer_CloseInGoal,
    (void *)Pursuer_Chase1A0, (void *)Pursuer_Chase1A4, (void *)Pursuer_ChaseTarget, (void *)Pursuer_DoorLineUp,
    (void *)Pursuer_DoorGoThrough, (void *)Pursuer_DoorFace, (void *)Pursuer_DoorOpen, (void *)Pursuer_DoorThrough,
    (void *)Pursuer_DoorPush, (void *)Pursuer_Door1C4, (void *)Pursuer_Door1C8, (void *)Pursuer_DoorWalkTo,
    (void *)Pursuer_DoorWalking, (void *)Pursuer_DoorFacing, (void *)Pursuer_DoorBarge, (void *)Pursuer_DoorWait,
    (void *)Pursuer_DoorIdle, (void *)Pursuer_Door1E4, (void *)Pursuer_ExitThrough, (void *)Pursuer_ExitOpen,
    (void *)Pursuer_Exit1F0, (void *)Pursuer_ExitGoTo, (void *)Pursuer_ExitOpenStep, (void *)Pursuer_DoorThroughOpen,
    (void *)Kind39_ExitDone, (void *)Pursuer_DoorBackAway, (void *)Pursuer_Door208, (void *)Pursuer_DoorBackOff,
    (void *)Pursuer_DoorWalk, (void *)Pursuer_Door214, (void *)Pursuer_DoorPushThrough, (void *)Pursuer_WalkToSpot,
    (void *)Pursuer_DoorArmOpen, (void *)Pursuer_Stairs, (void *)Pursuer_DoorAnim, (void *)Pursuer_DoorApproach,
    (void *)Pursuer_WalkAside, (void *)Pursuer_LookAround, (void *)Pursuer_SearchRoute, (void *)Pursuer_ReactToEvent,
    (void *)Pursuer_StepCount, (void *)Pursuer_FollowPathExit, (void *)Pursuer_Arrived, (void *)Pursuer_WalkToExit,
    (void *)Pursuer_OnToNextExit, (void *)Pursuer_NextPathExit, (void *)Pursuer_GoToDoor, (void *)Pursuer_Behaviour25C,
    (void *)Pursuer_BehaviourSearch, (void *)Pursuer_ChaseDecision, (void *)Pursuer_BehaviourStalk, (void *)Pursuer_BehaviourFollow,
    (void *)Pursuer_GoForHewie, (void *)Pursuer_BehaviourHewie, (void *)Pursuer_ChaseFiona, (void *)Pursuer_BehaviourRun,
    (void *)Pursuer_GoAfterFiona, (void *)Pursuer_BehaviourDoors, (void *)Pursuer_Behaviour288, (void *)Pursuer_BehaviourAttack,
    (void *)Pursuer_BehaviourIdle, (void *)Pursuer_OffscreenStep, (void *)Pursuer_OffscreenUpdate, (void *)Pursuer_BehaviourEnded,
    (void *)Pursuer_FrameUpdate, (void *)Pursuer_ClearBehaviour, (void *)Pursuer_GiveUp, (void *)Pursuer_GoForFiona,
    (void *)Pursuer_HeadingStep, (void *)Pursuer_FreshStart, (void *)Pursuer_CarryOn, (void *)Pursuer_Caught,
    (void *)Pursuer_Timer10s, (void *)Pursuer_Timer15s, (void *)Pursuer_SetTimer, (void *)Pursuer_NoGiveUp,
    (void *)Pursuer_Get2D0, (void *)Pursuer_Get2D4, (void *)Kind39_ActionOffsets, (void *)Pursuer_LookFrames,
    (void *)Pursuer_LookSwing, (void *)Pursuer_ReachFiona, (void *)Pursuer_Dist2E8, (void *)Pursuer_AttackAngle,
    (void *)Kind39_ReachHewie, (void *)Kind39_AttackRange, (void *)Pursuer_SpeedTop, (void *)Pursuer_SpeedBase,
    (void *)Kind39_FrightSeen, (void *)Kind39_FrightAttack, (void *)Kind39_ThreatAmount, (void *)Kind39_AttackAnimA,
    (void *)Kind39_AttackAnimB, (void *)Pursuer_RoomSpots, (void *)Pursuer_Get318, (void *)Pursuer_SetRage,
    (void *)Pursuer_WalkAnim, (void *)Pursuer_StanceAnim, (void *)Pursuer_SlowWalkAnim, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Spark_dtor, (void *)Spark_Start,
    (void *)Spark_Update, (void *)Spark_Draw, (void *)Spark_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Thing09_dtor, (void *)Thing09_Setup,
    (void *)Actor_Cleanup, (void *)Actor_LoadFiles, (void *)Actor_FilesLoading, (void *)Actor_FilesLoaded,
    (void *)Actor_Unload, (void *)Actor_RememberPos, (void *)Actor_Place, (void *)Thing09_Draw,
    (void *)Thing09_Frame, (void *)Actor_LeaveScreen, (void *)Actor_ShowUp, (void *)Actor_CanAct,
    (void *)Thing_PutAt, (void *)Thing_PutDown, (void *)Thing_LandTri, (void *)Thing09_MotionState,
    (void *)Thing09_Flying, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Thing10_dtor, (void *)Thing10_Setup,
    (void *)Actor_Cleanup, (void *)Actor_LoadFiles, (void *)Actor_FilesLoading, (void *)Actor_FilesLoaded,
    (void *)Actor_Unload, (void *)Actor_RememberPos, (void *)Actor_Place, (void *)Thing10_Draw,
    (void *)Thing10_Frame, (void *)Actor_LeaveScreen, (void *)Actor_ShowUp, (void *)Actor_CanAct,
    (void *)Thing_PutAt, (void *)Thing_PutDown, (void *)Thing_LandTri, (void *)Thing10_MotionState,
    (void *)Thing10_Flying, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SmokeTrail_dtor, (void *)SmokeTrail_Start,
    (void *)SmokeTrail_Update, (void *)SmokeTrail_Draw, (void *)SmokeTrail_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)WispColumn_dtor, (void *)WispColumn_Start,
    (void *)WispColumn_Update, (void *)WispColumn_Draw, (void *)WispColumn_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SparkSpray_dtor, (void *)SparkSpray_Start,
    (void *)SparkSpray_Update, (void *)SparkSpray_Draw, (void *)SparkSpray_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SpringPart_Reset, (void *)SpringPart_NoopC,
    (void *)WindHangPoint_Frame, (void *)WindHangPoint_AdjustBone, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room37_dtor, (void *)Room37_EnterScript,
    (void *)Room37_Phase1Script, (void *)Room37_Phase2Script, (void *)Room37_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)Room37_ActionScript, (void *)Room37_Command, (void *)RoomBase_Condition,
    (void *)Room37_CharEnterScript, (void *)Room37_ObjectName, (void *)RoomBase_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Effect79FF0_dtor, (void *)Effect79FF0_Start,
    (void *)Effect79FF0_Update, (void *)Effect79FF0_Draw, (void *)Effect79FF0_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)BurstA_dtor, (void *)BurstA_Start,
    (void *)BurstA_Update, (void *)BurstA_Draw, (void *)BurstA_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)BurstB_dtor, (void *)BurstB_Start,
    (void *)BurstB_Update, (void *)BurstB_Draw, (void *)BurstB_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)ThingPuff_dtor, (void *)ThingPuff_Start,
    (void *)ThingPuff_Update, (void *)ThingPuff_Draw, (void *)ThingPuff_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomD0_dtor, (void *)RoomD0_EnterScript,
    (void *)RoomD0_Phase1Script, (void *)RoomD0_Phase2Script, (void *)RoomD0_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomD0_ActionScript, (void *)RoomD0_Command, (void *)RoomD0_Condition,
    (void *)RoomD0_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomD0_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomD1_dtor, (void *)RoomD1_EnterScript,
    (void *)RoomD1_Phase1Script, (void *)RoomD1_Phase2Script, (void *)RoomD1_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomD1_ActionScript, (void *)RoomD1_Command, (void *)RoomD1_Condition,
    (void *)RoomD1_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomD1_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomD2_dtor, (void *)RoomD2_EnterScript,
    (void *)RoomD2_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomD2_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomBase_ActionScript, (void *)RoomD2_Command, (void *)RoomD2_Condition,
    (void *)RoomD2_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomD2_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomD3_dtor, (void *)RoomD3_EnterScript,
    (void *)RoomD3_Phase1Script, (void *)RoomD3_Phase2Script, (void *)RoomD3_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomD3_ActionScript, (void *)RoomD3_Command, (void *)RoomD3_Condition,
    (void *)RoomD3_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomD3_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomD4_dtor, (void *)RoomD4_EnterScript,
    (void *)RoomD4_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomD4_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomBase_ActionScript, (void *)RoomD4_Command, (void *)RoomD4_Condition,
    (void *)RoomD4_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomD4_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomD5_dtor, (void *)RoomD5_EnterScript,
    (void *)RoomD5_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomD5_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomD5_Phase5Script, (void *)RoomD5_ActionScript, (void *)RoomD5_Command, (void *)RoomD5_Condition,
    (void *)RoomD5_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomD5_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomD6_dtor, (void *)RoomD6_EnterScript,
    (void *)RoomD6_Phase1Script, (void *)RoomD6_Phase2Script, (void *)RoomD6_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomD6_Phase5Script, (void *)RoomD6_ActionScript, (void *)RoomD6_Command, (void *)RoomD6_Condition,
    (void *)RoomD6_CharEnterScript, (void *)RoomD6_ObjectName, (void *)RoomD6_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomD7_dtor, (void *)RoomD7_EnterScript,
    (void *)RoomD7_Phase1Script, (void *)RoomD7_Phase2Script, (void *)RoomD7_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomD7_Phase5Script, (void *)RoomD7_ActionScript, (void *)RoomD7_Command, (void *)RoomD7_Condition,
    (void *)RoomD7_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomD7_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomD8_dtor, (void *)RoomD8_EnterScript,
    (void *)RoomD8_Phase1Script, (void *)RoomD8_Phase2Script, (void *)RoomD8_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomD8_Phase5Script, (void *)RoomD8_ActionScript, (void *)RoomD8_Command, (void *)RoomD8_Condition,
    (void *)RoomD8_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomD8_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomD9_dtor, (void *)RoomD9_EnterScript,
    (void *)RoomD9_Phase1Script, (void *)RoomBase_Phase2Script, (void *)RoomD9_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomD9_ActionScript, (void *)RoomD9_Command, (void *)RoomD9_Condition,
    (void *)RoomD9_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomD9_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room92Effect_dtor, (void *)Room92Effect_Start,
    (void *)Room92Effect_Update, (void *)Room92Effect_Draw, (void *)Room92Effect_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Embers_dtor, (void *)Embers_Start,
    (void *)Embers_Update, (void *)Embers_Draw, (void *)Embers_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SceneEnding_dtor, (void *)Scene_Update,
    (void *)SceneEnding_StateEntry, (void *)SceneEnding_Finish, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)LorenzoSpark_dtor, (void *)LorenzoSpark_Start,
    (void *)LorenzoSpark_Update, (void *)LorenzoSpark_Draw, (void *)LorenzoSpark_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)LightShaft_dtor, (void *)LightShaft_Start,
    (void *)LightShaft_Update, (void *)LightShaft_Draw, (void *)LightShaft_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Room49Effect_dtor, (void *)Room49Effect_Start,
    (void *)Room49Effect_Update, (void *)Room49Effect_Draw, (void *)EffectBase_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Glint_dtor, (void *)Glint_Start,
    (void *)Glint_Update, (void *)Glint_Draw, (void *)EffectBase_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)Effect7A3D0_dtor, (void *)Effect7A3D0_Start,
    (void *)Effect7A3D0_Update, (void *)Effect7A3D0_Draw, (void *)EffectBase_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)BackdropModel_dtor, (void *)BackdropModel_Start,
    (void *)BackdropModel_Update, (void *)BackdropModel_Draw, (void *)EffectBase_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)TurningModel_dtor, (void *)TurningModel_Start,
    (void *)TurningModel_Update, (void *)TurningModel_Draw, (void *)EffectBase_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)BackdropModel2_dtor, (void *)BackdropModel2_Start,
    (void *)BackdropModel2_Update, (void *)BackdropModel2_Draw, (void *)BackdropModel2_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomE0_dtor, (void *)RoomE0_EnterScript,
    (void *)RoomE0_Phase1Script, (void *)RoomE0_Phase2Script, (void *)RoomE0_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomE0_Phase5Script, (void *)RoomE0_ActionScript, (void *)RoomE0_Command, (void *)RoomE0_Condition,
    (void *)RoomE0_CharEnterScript, (void *)RoomE0_ObjectName, (void *)RoomE0_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomE1_dtor, (void *)RoomE1_EnterScript,
    (void *)RoomE1_Phase1Script, (void *)RoomE1_Phase2Script, (void *)RoomE1_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomE1_ActionScript, (void *)RoomE1_Command, (void *)RoomE1_Condition,
    (void *)RoomE1_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomE1_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomE2_dtor, (void *)RoomE2_EnterScript,
    (void *)RoomE2_Phase1Script, (void *)RoomE2_Phase2Script, (void *)RoomE2_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomE2_ActionScript, (void *)RoomE2_Command, (void *)RoomE2_Condition,
    (void *)RoomE2_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomE2_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomE3_dtor, (void *)RoomE3_EnterScript,
    (void *)RoomE3_Phase1Script, (void *)RoomE3_Phase2Script, (void *)RoomE3_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomE3_Phase5Script, (void *)RoomE3_ActionScript, (void *)RoomE3_Command, (void *)RoomE3_Condition,
    (void *)RoomE3_CharEnterScript, (void *)RoomE3_ObjectName, (void *)RoomE3_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomE4_dtor, (void *)RoomE4_EnterScript,
    (void *)RoomE4_Phase1Script, (void *)RoomE4_Phase2Script, (void *)RoomE4_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomE4_ActionScript, (void *)RoomE4_Command, (void *)RoomE4_Condition,
    (void *)RoomE4_CharEnterScript, (void *)RoomE4_ObjectName, (void *)RoomE4_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomE5_dtor, (void *)RoomE5_EnterScript,
    (void *)RoomE5_Phase1Script, (void *)RoomE5_Phase2Script, (void *)RoomE5_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomE5_Phase5Script, (void *)RoomE5_ActionScript, (void *)RoomE5_Command, (void *)RoomE5_Condition,
    (void *)RoomE5_CharEnterScript, (void *)RoomE5_ObjectName, (void *)RoomE5_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomE6_dtor, (void *)RoomE6_EnterScript,
    (void *)RoomE6_Phase1Script, (void *)RoomE6_Phase2Script, (void *)RoomE6_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomE6_ActionScript, (void *)RoomE6_Command, (void *)RoomE6_Condition,
    (void *)RoomE6_CharEnterScript, (void *)RoomE6_ObjectName, (void *)RoomE6_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomE7_dtor, (void *)RoomE7_EnterScript,
    (void *)RoomE7_Phase1Script, (void *)RoomE7_Phase2Script, (void *)RoomE7_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomE7_Phase5Script, (void *)RoomE7_ActionScript, (void *)RoomE7_Command, (void *)RoomE7_Condition,
    (void *)RoomE7_CharEnterScript, (void *)RoomE7_ObjectName, (void *)RoomE7_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomE8_dtor, (void *)RoomE8_EnterScript,
    (void *)RoomE8_Phase1Script, (void *)RoomE8_Phase2Script, (void *)RoomE8_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomE8_ActionScript, (void *)RoomE8_Command, (void *)RoomE8_Condition,
    (void *)RoomE8_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomE8_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)RoomE9_dtor, (void *)RoomE9_EnterScript,
    (void *)RoomE9_Phase1Script, (void *)RoomE9_Phase2Script, (void *)RoomE9_Phase3Script, (void *)RoomBase_Phase4Script,
    (void *)RoomBase_Phase5Script, (void *)RoomE9_ActionScript, (void *)RoomE9_Command, (void *)RoomE9_Condition,
    (void *)RoomE9_CharEnterScript, (void *)RoomBase_ObjectName, (void *)RoomE9_Table38, (void *)RoomBase_Table3C,
    (void *)0x00000000u, (void *)0x00000000u, (void *)DustMote_dtor, (void *)DustMote_Start,
    (void *)DustMote_Update, (void *)DustMote_Draw, (void *)DustMote_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)DustMoteSource_dtor, (void *)DustMoteSource_Start,
    (void *)DustMoteSource_Update, (void *)DustMoteSource_Draw, (void *)DustMoteSource_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)DropletFlash_dtor, (void *)DropletFlash_Start,
    (void *)DropletFlash_Update, (void *)DropletFlash_Draw, (void *)DropletFlash_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)AshFlakes_dtor, (void *)AshFlakes_Start,
    (void *)AshFlakes_Update, (void *)AshFlakes_Draw, (void *)EffectBase_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)LightRing_dtor, (void *)LightRing_Start,
    (void *)LightRing_Update, (void *)LightRing_Draw, (void *)LightRing_SetParams, (void *)EffectBase_Query,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SceneBoot_dtor, (void *)Scene_Update,
    (void *)SceneBoot_StateEntry, (void *)Scene_OnSoftReset, (void *)0x00000000u, (void *)0x00000000u,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SubScreen_dtor, (void *)SubScreen_AddFile,
    (void *)func_003854B0, (void *)SubScreen_CostumeModel, (void *)SubScreen_Costume, (void *)SubScreen_UseEquipped,
    (void *)SubScreen_TabCommand, (void *)SubScreen_DrawTab, (void *)SubScreen_TakeOver, (void *)SubScreen_Part97980,
    (void *)SubScreen_Byte97A8F, (void *)SubScreen_SetLayout, (void *)SubScreen_SetBit, (void *)SubScreen_TestBit,
    (void *)0x00000000u, (void *)0x00000000u, (void *)SceneGame_dtor, (void *)SceneGame_Update,
    (void *)SceneGame_StateEntry, (void *)SceneGame_OnSoftReset, (void *)0x00000000u, (void *)0x00000000u,
    (void *)func_003A1B70, (void *)func_003A1B60, (void *)func_003A1B50, (void *)func_003A1B30,
    (void *)func_003A1B40, (void *)func_003A1B20, (void *)func_003A1B10, (void *)func_003A1B00,
    (void *)func_003A1AF0, (void *)Progress_CharBoneCall, (void *)Progress_CharNear, (void *)Progress_SlotAfresh,
    (void *)func_003A1AE0, (void *)func_003A1AD0, (void *)func_003A1AC0, (void *)func_003A1AB0,
    (void *)func_003A1AA0, (void *)func_003A1A90, (void *)func_003A1A80, (void *)func_003A1A70,
    (void *)func_003A1A60, (void *)func_003A1A50, (void *)func_003A1A40, (void *)Progress_StalkerAlert,
    (void *)func_003A1A30, (void *)func_003A1A20, (void *)func_003A1A10, (void *)func_003A1A00,
    (void *)func_003A19F0, (void *)func_003A19E0, (void *)func_003A19D0, (void *)func_003A19C0,
    (void *)func_003A19B0, (void *)SceneGame_CurrentRoom, (void *)SceneGame_LeaveRoom, (void *)SceneGame_PrepareExit,
    (void *)SceneGame_LoadSpareRoom, (void *)SceneGame_SetRoomBit, (void *)SceneGame_SubScreenStart, (void *)SceneGame_Save,
    (void *)SceneGame_Bits4to9, (void *)SceneGame_SetFlag3, (void *)SceneGame_PlaceCreature, (void *)SceneGame_SetCreatureCount,
    (void *)SceneGame_InPlay, (void *)SceneGame_State7, (void *)SceneGame_EventCamRunning, (void *)SceneGame_PlayStart,
    (void *)SceneGame_LockRoomDoors, (void *)SceneGame_UnlockRoomDoors, (void *)SceneGame_SetRoomDoors, (void *)SceneGame_RoomClosedTo,
    (void *)SceneGame_RandomRoomNear, (void *)SceneGame_RoomsNear, (void *)SceneGame_MusicDirector, (void *)SceneGame_EndMusic,
    (void *)SceneGame_Busy, (void *)SceneGame_BonusFlagsSet, (void *)SceneGame_BonusFlagCount, (void *)SceneGame_LoadSpareSlot,
    (void *)SceneGame_SpareRoomData, (void *)0x00000000u, (void *)0x00000000u, (void *)0x00000000u,
};
__asm__(G1_ALIAS("Sinit_Table", "g1_00469460", "0x0") G1_ALIAS("D_00469480", "g1_00469460", "0x20") G1_ALIAS("D_0046969C", "g1_00469460", "0x23C") G1_ALIAS("D_004696A0", "g1_00469460", "0x240") G1_ALIAS("D_004696B0", "g1_00469460", "0x250") G1_ALIAS("D_004696C0", "g1_00469460", "0x260") G1_ALIAS("D_004696D0", "g1_00469460", "0x270") G1_ALIAS("D_004696E0", "g1_00469460", "0x280") G1_ALIAS("D_004696F0", "g1_00469460", "0x290") G1_ALIAS("D_00469700", "g1_00469460", "0x2A0") G1_ALIAS("D_0046970C", "g1_00469460", "0x2AC") G1_ALIAS("D_00469718", "g1_00469460", "0x2B8") G1_ALIAS("D_00469730", "g1_00469460", "0x2D0") G1_ALIAS("D_0046973C", "g1_00469460", "0x2DC") G1_ALIAS("D_00469750", "g1_00469460", "0x2F0") G1_ALIAS("D_00469790", "g1_00469460", "0x330") G1_ALIAS("D_004697D0", "g1_00469460", "0x370") G1_ALIAS("D_004697DC", "g1_00469460", "0x37C") G1_ALIAS("D_004697E8", "g1_00469460", "0x388") G1_ALIAS("D_00469800", "g1_00469460", "0x3A0") G1_ALIAS("D_0046980C", "g1_00469460", "0x3AC") G1_ALIAS("D_00469820", "g1_00469460", "0x3C0") G1_ALIAS("D_00469860", "g1_00469460", "0x400") G1_ALIAS("D_004698A0", "g1_00469460", "0x440") G1_ALIAS("D_004698D0", "g1_00469460", "0x470") G1_ALIAS("D_00469900", "g1_00469460", "0x4A0") G1_ALIAS("D_00469940", "g1_00469460", "0x4E0") G1_ALIAS("D_00469950", "g1_00469460", "0x4F0") G1_ALIAS("D_00469960", "g1_00469460", "0x500") G1_ALIAS("D_00469990", "g1_00469460", "0x530") G1_ALIAS("Scene_vtable", "g1_00469460", "0x540") G1_ALIAS("BlockPool_vtable", "g1_00469460", "0x560") G1_ALIAS("SceneTableBase_vtable", "g1_00469460", "0x580") G1_ALIAS("Thing_vtable", "g1_00469460", "0x5A0") G1_ALIAS("Camera_vtable", "g1_00469460", "0x600") G1_ALIAS("CameraBase_vtable", "g1_00469460", "0x6E0") G1_ALIAS("Actor_vtable", "g1_00469460", "0x7C0") G1_ALIAS("Character_vtable", "g1_00469460", "0x800") G1_ALIAS("Helper469D00_vtable", "g1_00469460", "0x8A0") G1_ALIAS("Debilitas_vtable", "g1_00469460", "0x8B0") G1_ALIAS("SceneTitle_vtable", "g1_00469460", "0xBE0") G1_ALIAS("BootCard_vtable", "g1_00469460", "0xBF8") G1_ALIAS("TextObj_vtable", "g1_00469460", "0xC08") G1_ALIAS("SubPool_vtable", "g1_00469460", "0xC18") G1_ALIAS("SubScreenBase_vtable", "g1_00469460", "0xC30") G1_ALIAS("MessageBase_vtable", "g1_00469460", "0xC70") G1_ALIAS("BgmCtlBase_vtable", "g1_00469460", "0xCA0") G1_ALIAS("BgmCtl_vtable", "g1_00469460", "0xCB0") G1_ALIAS("Hewie_vtable", "g1_00469460", "0xCC0") G1_ALIAS("Heap_vtable", "g1_00469460", "0xD60") G1_ALIAS("Loader_vtable", "g1_00469460", "0xD80") G1_ALIAS("LoaderBase_vtable", "g1_00469460", "0xDC0") G1_ALIAS("Progress_vtable", "g1_00469460", "0xE00") G1_ALIAS("Kind15_vtable", "g1_00469460", "0xE90") G1_ALIAS("Kind22_vtable", "g1_00469460", "0x11C0") G1_ALIAS("PlacedThingsBase_vtable", "g1_00469460", "0x14F0") G1_ALIAS("CreaturesBase_vtable", "g1_00469460", "0x1520") G1_ALIAS("Obj46A9B0_vtable", "g1_00469460", "0x1550") G1_ALIAS("Obj46A9C0_vtable", "g1_00469460", "0x1560") G1_ALIAS("NavMesh_vtable", "g1_00469460", "0x1570") G1_ALIAS("NavMeshBase_vtable", "g1_00469460", "0x15E0") G1_ALIAS("Fiona_vtable", "g1_00469460", "0x1650") G1_ALIAS("Random_vtable", "g1_00469460", "0x16F0") G1_ALIAS("RandomBase_vtable", "g1_00469460", "0x1720") G1_ALIAS("PathPlan_vtable", "g1_00469460", "0x1750") G1_ALIAS("PathPlanBase_vtable", "g1_00469460", "0x17A0") G1_ALIAS("Renderer_vtable", "g1_00469460", "0x17F0") G1_ALIAS("RendererBase_vtable", "g1_00469460", "0x1890") G1_ALIAS("Obj46AD88_vtable", "g1_00469460", "0x1928") G1_ALIAS("ModelDrawer_vtable", "g1_00469460", "0x1940") G1_ALIAS("Pads_vtable", "g1_00469460", "0x1950") G1_ALIAS("Pads_vtable18", "g1_00469460", "0x1964") G1_ALIAS("PadsBase_vtable", "g1_00469460", "0x1970") G1_ALIAS("System_vtable", "g1_00469460", "0x1990") G1_ALIAS("SystemBase_vtable", "g1_00469460", "0x19B0") G1_ALIAS("RumbleBase_vtable", "g1_00469460", "0x19D0") G1_ALIAS("MemCardBase_vtable", "g1_00469460", "0x1A00") G1_ALIAS("MemCard_vtable", "g1_00469460", "0x1A30") G1_ALIAS("MemCard_subVtable", "g1_00469460", "0x1A54") G1_ALIAS("AdxBase_vtable", "g1_00469460", "0x1A60") G1_ALIAS("MovieLibBase_vtable", "g1_00469460", "0x1A70") G1_ALIAS("MovieSys_vtable", "g1_00469460", "0x1AA0") G1_ALIAS("MovieSys_vtable124", "g1_00469460", "0x1AAC") G1_ALIAS("VramBase_vtable", "g1_00469460", "0x1AC0") G1_ALIAS("SndDriverBase_vtable", "g1_00469460", "0x1B30") G1_ALIAS("Vram_vtable", "g1_00469460", "0x1BF0") G1_ALIAS("IK3_vtable", "g1_00469460", "0x1C60") G1_ALIAS("IK2_vtable", "g1_00469460", "0x1C70") G1_ALIAS("CharModel_vtable", "g1_00469460", "0x1C80") G1_ALIAS("Shadow_vtable", "g1_00469460", "0x1D60") G1_ALIAS("TexCache_vtable", "g1_00469460", "0x1D70") G1_ALIAS("TexCacheBase_vtable", "g1_00469460", "0x1D90") G1_ALIAS("ModelBase_vtable", "g1_00469460", "0x1DB0") G1_ALIAS("DogModel_vtable", "g1_00469460", "0x1DE0") G1_ALIAS("Lights_vtable", "g1_00469460", "0x1EA0") G1_ALIAS("LightsBase_vtable", "g1_00469460", "0x1EF0") G1_ALIAS("Events_vtable", "g1_00469460", "0x1F40") G1_ALIAS("Events_vtableC", "g1_00469460", "0x1F58") G1_ALIAS("Room101_vtable", "g1_00469460", "0x2050") G1_ALIAS("Room100_vtable", "g1_00469460", "0x2090") G1_ALIAS("Room82_vtable", "g1_00469460", "0x20D0") G1_ALIAS("Room67_vtable", "g1_00469460", "0x2110") G1_ALIAS("Room65_vtable", "g1_00469460", "0x2150") G1_ALIAS("Room64_vtable", "g1_00469460", "0x2190") G1_ALIAS("Room5F_vtable", "g1_00469460", "0x21D0") G1_ALIAS("Room5E_vtable", "g1_00469460", "0x2210") G1_ALIAS("Room5B_vtable", "g1_00469460", "0x2250") G1_ALIAS("Room4D_vtable", "g1_00469460", "0x2290") G1_ALIAS("Room44_vtable", "g1_00469460", "0x22D0") G1_ALIAS("Room42_vtable", "g1_00469460", "0x2310") G1_ALIAS("Room41_vtable", "g1_00469460", "0x2350") G1_ALIAS("Room16_vtable", "g1_00469460", "0x2390") G1_ALIAS("Room07_vtable", "g1_00469460", "0x23D0") G1_ALIAS("Room05_vtable", "g1_00469460", "0x2410") G1_ALIAS("Room01_vtable", "g1_00469460", "0x2450") G1_ALIAS("DogModelA_vtable", "g1_00469460", "0x2490") G1_ALIAS("DogModelB_vtable", "g1_00469460", "0x2550") G1_ALIAS("EventPoint_vtable", "g1_00469460", "0x2608") G1_ALIAS("Obj46BA80_vtable", "g1_00469460", "0x2620") G1_ALIAS("EventsBase_vtable", "g1_00469460", "0x2640") G1_ALIAS("CutsceneBase_vtable", "g1_00469460", "0x26C0") G1_ALIAS("Daniella_vtable", "g1_00469460", "0x2750") G1_ALIAS("GameBase_vtable", "g1_00469460", "0x2A80") G1_ALIAS("SceneTable_vtable", "g1_00469460", "0x2AA8") G1_ALIAS("SndDriver_vtable", "g1_00469460", "0x2AC0") G1_ALIAS("SndDriver_vtable4", "g1_00469460", "0x2ACC") G1_ALIAS("DebilitasModel_vtable", "g1_00469460", "0x2C40") G1_ALIAS("HumanModel_vtable", "g1_00469460", "0x2D00") G1_ALIAS("NPC_vtable", "g1_00469460", "0x2DC0") G1_ALIAS("Obstacles_vtable", "g1_00469460", "0x2EC0") G1_ALIAS("ObstaclesBase_vtable", "g1_00469460", "0x2F20") G1_ALIAS("Rooms_vtable", "g1_00469460", "0x2F80") G1_ALIAS("RoomsBase_vtable", "g1_00469460", "0x3020") G1_ALIAS("RoutePlanner_vtable", "g1_00469460", "0x30C0") G1_ALIAS("RoutePlannerBase_vtable", "g1_00469460", "0x30D0") G1_ALIAS("Doors_vtable", "g1_00469460", "0x30E0") G1_ALIAS("DoorsBase_vtable", "g1_00469460", "0x3170") G1_ALIAS("CamDirectorBase64_vtable", "g1_00469460", "0x3200") G1_ALIAS("CamDirector_vtable", "g1_00469460", "0x3208") G1_ALIAS("CamDirectorBase_vtable", "g1_00469460", "0x3290") G1_ALIAS("MovieLib_vtable", "g1_00469460", "0x32E0") G1_ALIAS("RoomMeshes_vtable", "g1_00469460", "0x3310") G1_ALIAS("PlacedMesh_vtable", "g1_00469460", "0x3320") G1_ALIAS("PoolEntry_vtable", "g1_00469460", "0x3330") G1_ALIAS("Item12_vtable", "g1_00469460", "0x3380") G1_ALIAS("Item13_vtable", "g1_00469460", "0x33D0") G1_ALIAS("Item3E_vtable", "g1_00469460", "0x3420") G1_ALIAS("PlainItem_vtable", "g1_00469460", "0x3470") G1_ALIAS("ItemClassC920_vtable", "g1_00469460", "0x34C0") G1_ALIAS("Item70_vtable", "g1_00469460", "0x3510") G1_ALIAS("Item71_vtable", "g1_00469460", "0x3560") G1_ALIAS("Item72_vtable", "g1_00469460", "0x35B0") G1_ALIAS("Item73_vtable", "g1_00469460", "0x3600") G1_ALIAS("Item74_vtable", "g1_00469460", "0x3650") G1_ALIAS("Item75_vtable", "g1_00469460", "0x36A0") G1_ALIAS("ItemClassCB50_vtable", "g1_00469460", "0x36F0") G1_ALIAS("Item80_vtable", "g1_00469460", "0x3740") G1_ALIAS("Item81_vtable", "g1_00469460", "0x3790") G1_ALIAS("Item82_vtable", "g1_00469460", "0x37E0") G1_ALIAS("Item83_vtable", "g1_00469460", "0x3830") G1_ALIAS("ItemClassCCE0_vtable", "g1_00469460", "0x3880") G1_ALIAS("Item86_vtable", "g1_00469460", "0x38D0") G1_ALIAS("Item87_vtable", "g1_00469460", "0x3920") G1_ALIAS("Item88_vtable", "g1_00469460", "0x3970") G1_ALIAS("Item89_vtable", "g1_00469460", "0x39C0") G1_ALIAS("ItemClassCE70_vtable", "g1_00469460", "0x3A10") G1_ALIAS("Item8A_vtable", "g1_00469460", "0x3A60") G1_ALIAS("Item8B_vtable", "g1_00469460", "0x3AB0") G1_ALIAS("Item8C_vtable", "g1_00469460", "0x3B00") G1_ALIAS("Item8D_vtable", "g1_00469460", "0x3B50") G1_ALIAS("ItemClassD000_vtable", "g1_00469460", "0x3BA0") G1_ALIAS("Item90_vtable", "g1_00469460", "0x3BF0") G1_ALIAS("Item91_vtable", "g1_00469460", "0x3C40") G1_ALIAS("Item92_vtable", "g1_00469460", "0x3C90") G1_ALIAS("Item93_vtable", "g1_00469460", "0x3CE0") G1_ALIAS("Item94_vtable", "g1_00469460", "0x3D30") G1_ALIAS("Item95_vtable", "g1_00469460", "0x3D80") G1_ALIAS("Item97_vtable", "g1_00469460", "0x3DD0") G1_ALIAS("Item98_vtable", "g1_00469460", "0x3E20") G1_ALIAS("Item9B_vtable", "g1_00469460", "0x3E70") G1_ALIAS("ItemClassD320_vtable", "g1_00469460", "0x3EC0") G1_ALIAS("ItemA4_vtable", "g1_00469460", "0x3F10") G1_ALIAS("ItemA5_vtable", "g1_00469460", "0x3F60") G1_ALIAS("ItemA6_vtable", "g1_00469460", "0x3FB0") G1_ALIAS("ItemA7_vtable", "g1_00469460", "0x4000") G1_ALIAS("ItemA8_vtable", "g1_00469460", "0x4050") G1_ALIAS("ItemA9_vtable", "g1_00469460", "0x40A0") G1_ALIAS("ItemAA_vtable", "g1_00469460", "0x40F0") G1_ALIAS("ItemAB_vtable", "g1_00469460", "0x4140") G1_ALIAS("ItemAC_vtable", "g1_00469460", "0x4190") G1_ALIAS("ItemClassD640_vtable", "g1_00469460", "0x41E0") G1_ALIAS("Item40_vtable", "g1_00469460", "0x4230") G1_ALIAS("Item41_vtable", "g1_00469460", "0x4280") G1_ALIAS("RoomEffectBase_vtable", "g1_00469460", "0x42D0") G1_ALIAS("Tint_vtable", "g1_00469460", "0x42F0") G1_ALIAS("PlacedModelBase_vtable", "g1_00469460", "0x4310") G1_ALIAS("Obj46D780_vtable", "g1_00469460", "0x4320") G1_ALIAS("TintDrawer_vtable", "g1_00469460", "0x4330") G1_ALIAS("Bloom_vtable", "g1_00469460", "0x4340") G1_ALIAS("ScreenBlend_vtable", "g1_00469460", "0x4350") G1_ALIAS("Message_vtable", "g1_00469460", "0x4370") G1_ALIAS("DoorShadow_vtable", "g1_00469460", "0x43A0") G1_ALIAS("Pursuer_vtable", "g1_00469460", "0x43B0") G1_ALIAS("NavGroups_vtable", "g1_00469460", "0x46E0") G1_ALIAS("NavGroupsBase_vtable", "g1_00469460", "0x4700") G1_ALIAS("RoomBase_vtable", "g1_00469460", "0x4720") G1_ALIAS("Room00_vtable", "g1_00469460", "0x4760") G1_ALIAS("Room02_vtable", "g1_00469460", "0x47A0") G1_ALIAS("Room03_vtable", "g1_00469460", "0x47E0") G1_ALIAS("Room04_vtable", "g1_00469460", "0x4820") G1_ALIAS("Room06_vtable", "g1_00469460", "0x4860") G1_ALIAS("Room08_vtable", "g1_00469460", "0x48A0") G1_ALIAS("Room09_vtable", "g1_00469460", "0x48E0") G1_ALIAS("Room0A_vtable", "g1_00469460", "0x4920") G1_ALIAS("Room0B_vtable", "g1_00469460", "0x4960") G1_ALIAS("Room0C_vtable", "g1_00469460", "0x49A0") G1_ALIAS("Room0D_vtable", "g1_00469460", "0x49E0") G1_ALIAS("Room0E_vtable", "g1_00469460", "0x4A20") G1_ALIAS("Room0F_vtable", "g1_00469460", "0x4A60") G1_ALIAS("Room10_vtable", "g1_00469460", "0x4AA0") G1_ALIAS("Room11_vtable", "g1_00469460", "0x4AE0") G1_ALIAS("Room12_vtable", "g1_00469460", "0x4B20") G1_ALIAS("Room13_vtable", "g1_00469460", "0x4B60") G1_ALIAS("Room14_vtable", "g1_00469460", "0x4BA0") G1_ALIAS("Room15_vtable", "g1_00469460", "0x4BE0") G1_ALIAS("Room18_vtable", "g1_00469460", "0x4C20") G1_ALIAS("Room19_vtable", "g1_00469460", "0x4C60") G1_ALIAS("Room1A_vtable", "g1_00469460", "0x4CA0") G1_ALIAS("Room1B_vtable", "g1_00469460", "0x4CE0") G1_ALIAS("Room1C_vtable", "g1_00469460", "0x4D20") G1_ALIAS("Room1D_vtable", "g1_00469460", "0x4D60") G1_ALIAS("Room1E_vtable", "g1_00469460", "0x4DA0") G1_ALIAS("Room1F_vtable", "g1_00469460", "0x4DE0") G1_ALIAS("Room20_vtable", "g1_00469460", "0x4E20") G1_ALIAS("Room21_vtable", "g1_00469460", "0x4E60") G1_ALIAS("Room22_vtable", "g1_00469460", "0x4EA0") G1_ALIAS("Room23_vtable", "g1_00469460", "0x4EE0") G1_ALIAS("Room24_vtable", "g1_00469460", "0x4F20") G1_ALIAS("Room25_vtable", "g1_00469460", "0x4F60") G1_ALIAS("Room26_vtable", "g1_00469460", "0x4FA0") G1_ALIAS("Room28_vtable", "g1_00469460", "0x4FE0") G1_ALIAS("Room29_vtable", "g1_00469460", "0x5020") G1_ALIAS("Room2A_vtable", "g1_00469460", "0x5060") G1_ALIAS("Room2B_vtable", "g1_00469460", "0x50A0") G1_ALIAS("Room2D_vtable", "g1_00469460", "0x50E0") G1_ALIAS("Room40_vtable", "g1_00469460", "0x5120") G1_ALIAS("Room43_vtable", "g1_00469460", "0x5160") G1_ALIAS("Room45_vtable", "g1_00469460", "0x51A0") G1_ALIAS("Room49_vtable", "g1_00469460", "0x51E0") G1_ALIAS("Room4A_vtable", "g1_00469460", "0x5220") G1_ALIAS("Room4B_vtable", "g1_00469460", "0x5260") G1_ALIAS("Room4C_vtable", "g1_00469460", "0x52A0") G1_ALIAS("Room4E_vtable", "g1_00469460", "0x52E0") G1_ALIAS("Room4F_vtable", "g1_00469460", "0x5320") G1_ALIAS("Room50_vtable", "g1_00469460", "0x5360") G1_ALIAS("Room51_vtable", "g1_00469460", "0x53A0") G1_ALIAS("Room52_vtable", "g1_00469460", "0x53E0") G1_ALIAS("Room56_vtable", "g1_00469460", "0x5420") G1_ALIAS("Room57_vtable", "g1_00469460", "0x5460") G1_ALIAS("Room58_vtable", "g1_00469460", "0x54A0") G1_ALIAS("Room59_vtable", "g1_00469460", "0x54E0") G1_ALIAS("Room5A_vtable", "g1_00469460", "0x5520") G1_ALIAS("Room5C_vtable", "g1_00469460", "0x5560") G1_ALIAS("Room5D_vtable", "g1_00469460", "0x55A0") G1_ALIAS("TvScreenA_vtable", "g1_00469460", "0x55E0") G1_ALIAS("Movie_vtable", "g1_00469460", "0x5600") G1_ALIAS("Butterflies_vtable", "g1_00469460", "0x5630") G1_ALIAS("MovieBlended_vtable", "g1_00469460", "0x5650") G1_ALIAS("MovieOpaque_vtable", "g1_00469460", "0x5680") G1_ALIAS("MovieHalf_vtable", "g1_00469460", "0x56B0") G1_ALIAS("Fog_vtable", "g1_00469460", "0x56E0") G1_ALIAS("FogDrawer_vtable", "g1_00469460", "0x5700") G1_ALIAS("MusicDir_vtable", "g1_00469460", "0x5710") G1_ALIAS("MusicDirBase_vtable", "g1_00469460", "0x5780") G1_ALIAS("MovieCopied_vtable", "g1_00469460", "0x57D0") G1_ALIAS("DepthRange_vtable", "g1_00469460", "0x5800") G1_ALIAS("DofDrawer_vtable", "g1_00469460", "0x5820") G1_ALIAS("MovieAdded_vtable", "g1_00469460", "0x5830") G1_ALIAS("MovieScene_vtable", "g1_00469460", "0x5860") G1_ALIAS("PlacedObjects_vtable", "g1_00469460", "0x5890") G1_ALIAS("Cutscene_vtable", "g1_00469460", "0x58D0") G1_ALIAS("Room2F_vtable", "g1_00469460", "0x5960") G1_ALIAS("Room30_vtable", "g1_00469460", "0x59A0") G1_ALIAS("Item00_vtable", "g1_00469460", "0x59E0") G1_ALIAS("Item01_vtable", "g1_00469460", "0x5A30") G1_ALIAS("Item02_vtable", "g1_00469460", "0x5A80") G1_ALIAS("Item03_vtable", "g1_00469460", "0x5AD0") G1_ALIAS("Item04_vtable", "g1_00469460", "0x5B20") G1_ALIAS("Item05_vtable", "g1_00469460", "0x5B70") G1_ALIAS("Debilitas3_vtable", "g1_00469460", "0x5BC0") G1_ALIAS("Overlay_vtable", "g1_00469460", "0x5EF0") G1_ALIAS("Game_vtable", "g1_00469460", "0x5F00") G1_ALIAS("PlacedObjectsBase_vtable", "g1_00469460", "0x5F30") G1_ALIAS("AvoidPromptBase_vtable", "g1_00469460", "0x5F70") G1_ALIAS("Room17_vtable", "g1_00469460", "0x5F90") G1_ALIAS("ItemClassF430_vtable", "g1_00469460", "0x5FD0") G1_ALIAS("MusicStage1_vtable", "g1_00469460", "0x6020") G1_ALIAS("Rumble_vtable", "g1_00469460", "0x6090") G1_ALIAS("Ball_vtable", "g1_00469460", "0x60C0") G1_ALIAS("EffectBase_vtable", "g1_00469460", "0x6120") G1_ALIAS("RisingSmoke_vtable", "g1_00469460", "0x6140") G1_ALIAS("PlacedThings_vtable", "g1_00469460", "0x6160") G1_ALIAS("TvScreenB_vtable", "g1_00469460", "0x6190") G1_ALIAS("ItemA0_vtable", "g1_00469460", "0x61B0") G1_ALIAS("ItemA1_vtable", "g1_00469460", "0x6200") G1_ALIAS("Riccardo_vtable", "g1_00469460", "0x6250") G1_ALIAS("Model_vtable", "g1_00469460", "0x6580") G1_ALIAS("CreatureA_vtable", "g1_00469460", "0x6640") G1_ALIAS("CreatureBase_vtable", "g1_00469460", "0x66F0") G1_ALIAS("Creatures_vtable", "g1_00469460", "0x67A0") G1_ALIAS("QuadDrawer_vtable", "g1_00469460", "0x67D0") G1_ALIAS("Room46_vtable", "g1_00469460", "0x67E0") G1_ALIAS("Room102_vtable", "g1_00469460", "0x6820") G1_ALIAS("Room103_vtable", "g1_00469460", "0x6860") G1_ALIAS("Room104_vtable", "g1_00469460", "0x68A0") G1_ALIAS("Room105_vtable", "g1_00469460", "0x68E0") G1_ALIAS("Room106_vtable", "g1_00469460", "0x6920") G1_ALIAS("Room107_vtable", "g1_00469460", "0x6960") G1_ALIAS("Room108_vtable", "g1_00469460", "0x69A0") G1_ALIAS("Room109_vtable", "g1_00469460", "0x69E0") G1_ALIAS("Room10A_vtable", "g1_00469460", "0x6A20") G1_ALIAS("Room10B_vtable", "g1_00469460", "0x6A60") G1_ALIAS("EvEffect7F_vtable", "g1_00469460", "0x6AA0") G1_ALIAS("SpriteBurst_vtable", "g1_00469460", "0x6AC0") G1_ALIAS("EvEffect86_vtable", "g1_00469460", "0x6AE0") G1_ALIAS("Effect6FF60_vtable", "g1_00469460", "0x6B00") G1_ALIAS("Kind08_vtable", "g1_00469460", "0x6B20") G1_ALIAS("D_0047000C", "g1_00469460", "0x6BAC") G1_ALIAS("D_004700A0", "g1_00469460", "0x6C40") G1_ALIAS("HangPoint_vtable", "g1_00469460", "0x6E50") G1_ALIAS("DaniellaModel_vtable", "g1_00469460", "0x6E70") G1_ALIAS("Capsule_vtable", "g1_00469460", "0x6F30") G1_ALIAS("BonePoint_vtable", "g1_00469460", "0x6F40") G1_ALIAS("SpringPartBase_vtable", "g1_00469460", "0x6F50") G1_ALIAS("BoneHangPoint_vtable", "g1_00469460", "0x6F70") G1_ALIAS("Item06_vtable", "g1_00469460", "0x6F90") G1_ALIAS("SprungPoint_vtable", "g1_00469460", "0x6FE0") G1_ALIAS("HairPoint_vtable", "g1_00469460", "0x7000") G1_ALIAS("RiccardoModel_vtable", "g1_00469460", "0x7020") G1_ALIAS("EventHumanModel_vtable", "g1_00469460", "0x70E0") G1_ALIAS("FionaModel_vtable", "g1_00469460", "0x71C0") G1_ALIAS("SwayPointA_vtable", "g1_00469460", "0x72A0") G1_ALIAS("Lorenzo_vtable", "g1_00469460", "0x72C0") G1_ALIAS("RisingMotes_vtable", "g1_00469460", "0x75F0") G1_ALIAS("OrangeSparks_vtable", "g1_00469460", "0x7610") G1_ALIAS("Debilitas2_vtable", "g1_00469460", "0x7630") G1_ALIAS("Room27_vtable", "g1_00469460", "0x7960") G1_ALIAS("SinkingSprite_vtable", "g1_00469460", "0x79A0") G1_ALIAS("SmokePuffs_vtable", "g1_00469460", "0x79C0") G1_ALIAS("SpeckSwarm_vtable", "g1_00469460", "0x79E0") G1_ALIAS("Splash_vtable", "g1_00469460", "0x7A00") G1_ALIAS("MovieOwnBuf_vtable", "g1_00469460", "0x7A20") G1_ALIAS("Room2C_vtable", "g1_00469460", "0x7A50") G1_ALIAS("Room2E_vtable", "g1_00469460", "0x7A90") G1_ALIAS("BloodSpray_vtable", "g1_00469460", "0x7AD0") G1_ALIAS("Room66_vtable", "g1_00469460", "0x7AF0") G1_ALIAS("Marker_vtable", "g1_00469460", "0x7B30") G1_ALIAS("Item07_vtable", "g1_00469460", "0x7B50") G1_ALIAS("Effect71000_vtable", "g1_00469460", "0x7BA0") G1_ALIAS("Room55_vtable", "g1_00469460", "0x7BC0") G1_ALIAS("DriftingFlecks_vtable", "g1_00469460", "0x7C00") G1_ALIAS("Item08_vtable", "g1_00469460", "0x7C20") G1_ALIAS("Item09_vtable", "g1_00469460", "0x7C70") G1_ALIAS("Item0A_vtable", "g1_00469460", "0x7CC0") G1_ALIAS("Item0B_vtable", "g1_00469460", "0x7D10") G1_ALIAS("Item0C_vtable", "g1_00469460", "0x7D60") G1_ALIAS("Room63_vtable", "g1_00469460", "0x7DB0") G1_ALIAS("Room62_vtable", "g1_00469460", "0x7DF0") G1_ALIAS("Kind09_vtable", "g1_00469460", "0x7E30") G1_ALIAS("Lorenzo2_vtable", "g1_00469460", "0x8160") G1_ALIAS("Kind12_vtable", "g1_00469460", "0x8490") G1_ALIAS("Kind09Model_vtable", "g1_00469460", "0x87C0") G1_ALIAS("Lorenzo2Model_vtable", "g1_00469460", "0x8880") G1_ALIAS("LorenzoModel_vtable", "g1_00469460", "0x8940") G1_ALIAS("Room68_vtable", "g1_00469460", "0x8A00") G1_ALIAS("Room47_vtable", "g1_00469460", "0x8A40") G1_ALIAS("Room48_vtable", "g1_00469460", "0x8A80") G1_ALIAS("Room53_vtable", "g1_00469460", "0x8AC0") G1_ALIAS("Room54_vtable", "g1_00469460", "0x8B00") G1_ALIAS("Room60_vtable", "g1_00469460", "0x8B40") G1_ALIAS("Room61_vtable", "g1_00469460", "0x8B80") G1_ALIAS("Kind13_vtable", "g1_00469460", "0x8BC0") G1_ALIAS("SwayPointB_vtable", "g1_00469460", "0x8EF0") G1_ALIAS("CreatureVanish_vtable", "g1_00469460", "0x8F10") G1_ALIAS("LoopingSprite_vtable", "g1_00469460", "0x8F30") G1_ALIAS("Kind14_vtable", "g1_00469460", "0x8F50") G1_ALIAS("Effect726E0_vtable", "g1_00469460", "0x9280") G1_ALIAS("Kind14Model_vtable", "g1_00469460", "0x92A0") G1_ALIAS("StrandSplash_vtable", "g1_00469460", "0x9360") G1_ALIAS("Thing01_vtable", "g1_00469460", "0x9380") G1_ALIAS("Thing02_vtable", "g1_00469460", "0x93E0") G1_ALIAS("Kind16_vtable", "g1_00469460", "0x9440") G1_ALIAS("HairPoint2_vtable", "g1_00469460", "0x9770") G1_ALIAS("FloorSplat_vtable", "g1_00469460", "0x9790") G1_ALIAS("HangingPart_vtable", "g1_00469460", "0x97B0") G1_ALIAS("Kind17_vtable", "g1_00469460", "0x97D0") G1_ALIAS("Reflection_vtable", "g1_00469460", "0x9B00") G1_ALIAS("Item0D_vtable", "g1_00469460", "0x9B20") G1_ALIAS("Item0E_vtable", "g1_00469460", "0x9B70") G1_ALIAS("D_00473000", "g1_00469460", "0x9BA0") G1_ALIAS("Item0F_vtable", "g1_00469460", "0x9BC0") G1_ALIAS("Item10_vtable", "g1_00469460", "0x9C10") G1_ALIAS("Item11_vtable", "g1_00469460", "0x9C60") G1_ALIAS("Kind18_vtable", "g1_00469460", "0x9CB0") G1_ALIAS("AvoidPrompt_vtable", "g1_00469460", "0x9FE0") G1_ALIAS("Room31_vtable", "g1_00469460", "0xA000") G1_ALIAS("Kind19_vtable", "g1_00469460", "0xA040") G1_ALIAS("Effect737D0_vtable", "g1_00469460", "0xA370") G1_ALIAS("Part50_vtable", "g1_00469460", "0xA390") G1_ALIAS("Part60_vtable", "g1_00469460", "0xA3B0") G1_ALIAS("MusicStage2_vtable", "g1_00469460", "0xA3D0") G1_ALIAS("TintStalker_vtable", "g1_00469460", "0xA440") G1_ALIAS("Kind23Model_vtable", "g1_00469460", "0xA770") G1_ALIAS("Room32_vtable", "g1_00469460", "0xA830") G1_ALIAS("Kind24_vtable", "g1_00469460", "0xA870") G1_ALIAS("Room2AWisps_vtable", "g1_00469460", "0xABA0") G1_ALIAS("SynthBase_vtable", "g1_00469460", "0xABC0") G1_ALIAS("SynthPot_vtable", "g1_00469460", "0xABE0") G1_ALIAS("SlotMachine_vtable", "g1_00469460", "0xAC00") G1_ALIAS("CreatureB_vtable", "g1_00469460", "0xAC20") G1_ALIAS("Kind25_vtable", "g1_00469460", "0xACD0") G1_ALIAS("Kind33Model_vtable", "g1_00469460", "0xB000") G1_ALIAS("Room86_vtable", "g1_00469460", "0xB0C0") G1_ALIAS("Kind21_vtable", "g1_00469460", "0xB100") G1_ALIAS("Kind20_vtable", "g1_00469460", "0xB430") G1_ALIAS("Item14_vtable", "g1_00469460", "0xB760") G1_ALIAS("Kind26_vtable", "g1_00469460", "0xB7B0") G1_ALIAS("RoomC0_vtable", "g1_00469460", "0xBAE0") G1_ALIAS("MovieSmall_vtable", "g1_00469460", "0xBB20") G1_ALIAS("ThingBurst_vtable", "g1_00469460", "0xBB50") G1_ALIAS("Kind27_vtable", "g1_00469460", "0xBB70") G1_ALIAS("Item42_vtable", "g1_00469460", "0xBEA0") G1_ALIAS("Item43_vtable", "g1_00469460", "0xBEF0") G1_ALIAS("Item44_vtable", "g1_00469460", "0xBF40") G1_ALIAS("Item45_vtable", "g1_00469460", "0xBF90") G1_ALIAS("Item46_vtable", "g1_00469460", "0xBFE0") G1_ALIAS("Item47_vtable", "g1_00469460", "0xC030") G1_ALIAS("Item48_vtable", "g1_00469460", "0xC080") G1_ALIAS("Item49_vtable", "g1_00469460", "0xC0D0") G1_ALIAS("Item4A_vtable", "g1_00469460", "0xC120") G1_ALIAS("Item4B_vtable", "g1_00469460", "0xC170") G1_ALIAS("Item4C_vtable", "g1_00469460", "0xC1C0") G1_ALIAS("Item60_vtable", "g1_00469460", "0xC210") G1_ALIAS("Item61_vtable", "g1_00469460", "0xC260") G1_ALIAS("Item62_vtable", "g1_00469460", "0xC2B0") G1_ALIAS("Item63_vtable", "g1_00469460", "0xC300") G1_ALIAS("Item64_vtable", "g1_00469460", "0xC350") G1_ALIAS("Item65_vtable", "g1_00469460", "0xC3A0") G1_ALIAS("Item66_vtable", "g1_00469460", "0xC3F0") G1_ALIAS("Thing03_vtable", "g1_00469460", "0xC440") G1_ALIAS("Thing06_vtable", "g1_00469460", "0xC4A0") G1_ALIAS("Thing05_vtable", "g1_00469460", "0xC500") G1_ALIAS("Thing07_vtable", "g1_00469460", "0xC560") G1_ALIAS("Thing08_vtable", "g1_00469460", "0xC5C0") G1_ALIAS("Thing04_vtable", "g1_00469460", "0xC620") G1_ALIAS("Costume2Model_vtable", "g1_00469460", "0xC680") G1_ALIAS("Costume3Model_vtable", "g1_00469460", "0xC760") G1_ALIAS("CostumeHangPoint_vtable", "g1_00469460", "0xC840") G1_ALIAS("Item15_vtable", "g1_00469460", "0xC860") G1_ALIAS("Item16_vtable", "g1_00469460", "0xC8B0") G1_ALIAS("Item17_vtable", "g1_00469460", "0xC900") G1_ALIAS("Kind28_vtable", "g1_00469460", "0xC950") G1_ALIAS("RoomC7_vtable", "g1_00469460", "0xCC80") G1_ALIAS("Kind29_vtable", "g1_00469460", "0xCCC0") G1_ALIAS("ItemA2_vtable", "g1_00469460", "0xCFF0") G1_ALIAS("Kind30_vtable", "g1_00469460", "0xD040") G1_ALIAS("Kind31_vtable", "g1_00469460", "0xD370") G1_ALIAS("ItemClass6B00_vtable", "g1_00469460", "0xD6A0") G1_ALIAS("FixModelDraw_vtable", "g1_00469460", "0xD6F0") G1_ALIAS("ItemA3_vtable", "g1_00469460", "0xD700") G1_ALIAS("DustShaft_vtable", "g1_00469460", "0xD750") G1_ALIAS("SpiralSmoke_vtable", "g1_00469460", "0xD770") G1_ALIAS("Drips_vtable", "g1_00469460", "0xD790") G1_ALIAS("Kind32_vtable", "g1_00469460", "0xD7B0") G1_ALIAS("LoadingEmblem_vtable", "g1_00469460", "0xDAE0") G1_ALIAS("Kind18Model_vtable", "g1_00469460", "0xDAF0") G1_ALIAS("Item18_vtable", "g1_00469460", "0xDBD0") G1_ALIAS("Item19_vtable", "g1_00469460", "0xDC20") G1_ALIAS("Item1A_vtable", "g1_00469460", "0xDC70") G1_ALIAS("Item1B_vtable", "g1_00469460", "0xDCC0") G1_ALIAS("Item1C_vtable", "g1_00469460", "0xDD10") G1_ALIAS("Room81_vtable", "g1_00469460", "0xDD60") G1_ALIAS("Room83_vtable", "g1_00469460", "0xDDA0") G1_ALIAS("Room84_vtable", "g1_00469460", "0xDDE0") G1_ALIAS("Room85_vtable", "g1_00469460", "0xDE20") G1_ALIAS("Room87_vtable", "g1_00469460", "0xDE60") G1_ALIAS("Room88_vtable", "g1_00469460", "0xDEA0") G1_ALIAS("Room8A_vtable", "g1_00469460", "0xDEE0") G1_ALIAS("Room8C_vtable", "g1_00469460", "0xDF20") G1_ALIAS("Room8D_vtable", "g1_00469460", "0xDF60") G1_ALIAS("Room8E_vtable", "g1_00469460", "0xDFA0") G1_ALIAS("Room8F_vtable", "g1_00469460", "0xDFE0") G1_ALIAS("Room91_vtable", "g1_00469460", "0xE020") G1_ALIAS("Room92_vtable", "g1_00469460", "0xE060") G1_ALIAS("Room97_vtable", "g1_00469460", "0xE0A0") G1_ALIAS("Room69_vtable", "g1_00469460", "0xE0E0") G1_ALIAS("Room6A_vtable", "g1_00469460", "0xE120") G1_ALIAS("Item1D_vtable", "g1_00469460", "0xE160") G1_ALIAS("Room6B_vtable", "g1_00469460", "0xE1B0") G1_ALIAS("Room6C_vtable", "g1_00469460", "0xE1F0") G1_ALIAS("Room6D_vtable", "g1_00469460", "0xE230") G1_ALIAS("Room6E_vtable", "g1_00469460", "0xE270") G1_ALIAS("Room6F_vtable", "g1_00469460", "0xE2B0") G1_ALIAS("Room70_vtable", "g1_00469460", "0xE2F0") G1_ALIAS("Kind33_vtable", "g1_00469460", "0xE330") G1_ALIAS("Room66Effect_vtable", "g1_00469460", "0xE660") G1_ALIAS("Kind34_vtable", "g1_00469460", "0xE680") G1_ALIAS("Room55Effect_vtable", "g1_00469460", "0xE9B0") G1_ALIAS("Kind35_vtable", "g1_00469460", "0xE9D0") G1_ALIAS("Kind36_vtable", "g1_00469460", "0xED00") G1_ALIAS("Costume6Model_vtable", "g1_00469460", "0xF030") G1_ALIAS("Room80_vtable", "g1_00469460", "0xF110") G1_ALIAS("RoomC1_vtable", "g1_00469460", "0xF150") G1_ALIAS("RoomC2_vtable", "g1_00469460", "0xF190") G1_ALIAS("RoomC3_vtable", "g1_00469460", "0xF1D0") G1_ALIAS("RoomC4_vtable", "g1_00469460", "0xF210") G1_ALIAS("RoomC5_vtable", "g1_00469460", "0xF250") G1_ALIAS("RoomC6_vtable", "g1_00469460", "0xF290") G1_ALIAS("RoomC8_vtable", "g1_00469460", "0xF2D0") G1_ALIAS("Kind37_vtable", "g1_00469460", "0xF310") G1_ALIAS("Room34_vtable", "g1_00469460", "0xF640") G1_ALIAS("MusicStage3_vtable", "g1_00469460", "0xF680") G1_ALIAS("Fire_vtable", "g1_00469460", "0xF6F0") G1_ALIAS("Caustic_vtable", "g1_00469460", "0xF710") G1_ALIAS("Room33_vtable", "g1_00469460", "0xF720") G1_ALIAS("Effect78BC0_vtable", "g1_00469460", "0xF760") G1_ALIAS("Room4EEffect_vtable", "g1_00469460", "0xF780") G1_ALIAS("Room98_vtable", "g1_00469460", "0xF7A0") G1_ALIAS("Room35_vtable", "g1_00469460", "0xF7E0") G1_ALIAS("Item1E_vtable", "g1_00469460", "0xF820") G1_ALIAS("Item1F_vtable", "g1_00469460", "0xF870") G1_ALIAS("Item20_vtable", "g1_00469460", "0xF8C0") G1_ALIAS("Item21_vtable", "g1_00469460", "0xF910") G1_ALIAS("Item22_vtable", "g1_00469460", "0xF960") G1_ALIAS("Item23_vtable", "g1_00469460", "0xF9B0") G1_ALIAS("Item24_vtable", "g1_00469460", "0xFA00") G1_ALIAS("Item25_vtable", "g1_00469460", "0xFA50") G1_ALIAS("Item26_vtable", "g1_00469460", "0xFAA0") G1_ALIAS("Item27_vtable", "g1_00469460", "0xFAF0") G1_ALIAS("Item28_vtable", "g1_00469460", "0xFB40") G1_ALIAS("Kind38_vtable", "g1_00469460", "0xFB90") G1_ALIAS("Wisps_vtable", "g1_00469460", "0xFEC0") G1_ALIAS("Room99_vtable", "g1_00469460", "0xFEE0") G1_ALIAS("Room9A_vtable", "g1_00469460", "0xFF20") G1_ALIAS("Room9B_vtable", "g1_00469460", "0xFF60") G1_ALIAS("FallingDrops_vtable", "g1_00469460", "0xFFA0") G1_ALIAS("Costume7Model_vtable", "g1_00469460", "0xFFC0") G1_ALIAS("ThingShared_vtable", "g1_00469460", "0x100A0") G1_ALIAS("MirrorFragment_vtable", "g1_00469460", "0x10100") G1_ALIAS("BigFire_vtable", "g1_00469460", "0x10120") G1_ALIAS("FloorGlow_vtable", "g1_00469460", "0x10140") G1_ALIAS("Room60Effect_vtable", "g1_00469460", "0x10160") G1_ALIAS("Cr19Bubbles_vtable", "g1_00469460", "0x10180") G1_ALIAS("StrikeMark_vtable", "g1_00469460", "0x101A0") G1_ALIAS("Room36_vtable", "g1_00469460", "0x101C0") G1_ALIAS("Costume8Model_vtable", "g1_00469460", "0x10200") G1_ALIAS("Kind12Model_vtable", "g1_00469460", "0x102E0") G1_ALIAS("BoneSmoke_vtable", "g1_00469460", "0x103A0") G1_ALIAS("Item29_vtable", "g1_00469460", "0x103C0") G1_ALIAS("ObjectGlow_vtable", "g1_00469460", "0x10410") G1_ALIAS("WallShadow_vtable", "g1_00469460", "0x10430") G1_ALIAS("RoomC7Cursor_vtable", "g1_00469460", "0x10450") G1_ALIAS("Room93_vtable", "g1_00469460", "0x10470") G1_ALIAS("Room94_vtable", "g1_00469460", "0x104B0") G1_ALIAS("Room95_vtable", "g1_00469460", "0x104F0") G1_ALIAS("Room96_vtable", "g1_00469460", "0x10530") G1_ALIAS("Debris_vtable", "g1_00469460", "0x10570") G1_ALIAS("MusicStage4_vtable", "g1_00469460", "0x10590") G1_ALIAS("CeilingDrips_vtable", "g1_00469460", "0x10600") G1_ALIAS("OneDrip_vtable", "g1_00469460", "0x10620") G1_ALIAS("DropletSpray_vtable", "g1_00469460", "0x10640") G1_ALIAS("WindowFlash_vtable", "g1_00469460", "0x10660") G1_ALIAS("SplashRing_vtable", "g1_00469460", "0x10680") G1_ALIAS("Effect79B00_vtable", "g1_00469460", "0x106A0") G1_ALIAS("Kind39_vtable", "g1_00469460", "0x106C0") G1_ALIAS("Spark_vtable", "g1_00469460", "0x109F0") G1_ALIAS("Thing09_vtable", "g1_00469460", "0x10A10") G1_ALIAS("Thing10_vtable", "g1_00469460", "0x10A70") G1_ALIAS("SmokeTrail_vtable", "g1_00469460", "0x10AD0") G1_ALIAS("WispColumn_vtable", "g1_00469460", "0x10AF0") G1_ALIAS("SparkSpray_vtable", "g1_00469460", "0x10B10") G1_ALIAS("WindHangPoint_vtable", "g1_00469460", "0x10B30") G1_ALIAS("Room37_vtable", "g1_00469460", "0x10B50") G1_ALIAS("Effect79FF0_vtable", "g1_00469460", "0x10B90") G1_ALIAS("BurstA_vtable", "g1_00469460", "0x10BB0") G1_ALIAS("BurstB_vtable", "g1_00469460", "0x10BD0") G1_ALIAS("ThingPuff_vtable", "g1_00469460", "0x10BF0") G1_ALIAS("RoomD0_vtable", "g1_00469460", "0x10C10") G1_ALIAS("RoomD1_vtable", "g1_00469460", "0x10C50") G1_ALIAS("RoomD2_vtable", "g1_00469460", "0x10C90") G1_ALIAS("RoomD3_vtable", "g1_00469460", "0x10CD0") G1_ALIAS("RoomD4_vtable", "g1_00469460", "0x10D10") G1_ALIAS("RoomD5_vtable", "g1_00469460", "0x10D50") G1_ALIAS("RoomD6_vtable", "g1_00469460", "0x10D90") G1_ALIAS("RoomD7_vtable", "g1_00469460", "0x10DD0") G1_ALIAS("RoomD8_vtable", "g1_00469460", "0x10E10") G1_ALIAS("RoomD9_vtable", "g1_00469460", "0x10E50") G1_ALIAS("Room92Effect_vtable", "g1_00469460", "0x10E90") G1_ALIAS("Embers_vtable", "g1_00469460", "0x10EB0") G1_ALIAS("SceneEnding_vtable", "g1_00469460", "0x10ED0") G1_ALIAS("LorenzoSpark_vtable", "g1_00469460", "0x10EF0") G1_ALIAS("LightShaft_vtable", "g1_00469460", "0x10F10") G1_ALIAS("Room49Effect_vtable", "g1_00469460", "0x10F30") G1_ALIAS("Glint_vtable", "g1_00469460", "0x10F50") G1_ALIAS("Effect7A3D0_vtable", "g1_00469460", "0x10F70") G1_ALIAS("BackdropModel_vtable", "g1_00469460", "0x10F90") G1_ALIAS("TurningModel_vtable", "g1_00469460", "0x10FB0") G1_ALIAS("BackdropModel2_vtable", "g1_00469460", "0x10FD0") G1_ALIAS("RoomE0_vtable", "g1_00469460", "0x10FF0") G1_ALIAS("RoomE1_vtable", "g1_00469460", "0x11030") G1_ALIAS("RoomE2_vtable", "g1_00469460", "0x11070") G1_ALIAS("RoomE3_vtable", "g1_00469460", "0x110B0") G1_ALIAS("RoomE4_vtable", "g1_00469460", "0x110F0") G1_ALIAS("RoomE5_vtable", "g1_00469460", "0x11130") G1_ALIAS("RoomE6_vtable", "g1_00469460", "0x11170") G1_ALIAS("RoomE7_vtable", "g1_00469460", "0x111B0") G1_ALIAS("RoomE8_vtable", "g1_00469460", "0x111F0") G1_ALIAS("RoomE9_vtable", "g1_00469460", "0x11230") G1_ALIAS("DustMote_vtable", "g1_00469460", "0x11270") G1_ALIAS("DustMoteSource_vtable", "g1_00469460", "0x11290") G1_ALIAS("DropletFlash_vtable", "g1_00469460", "0x112B0") G1_ALIAS("AshFlakes_vtable", "g1_00469460", "0x112D0") G1_ALIAS("LightRing_vtable", "g1_00469460", "0x112F0") G1_ALIAS("SceneBoot_vtable", "g1_00469460", "0x11310") G1_ALIAS("SubScreen_vtable", "g1_00469460", "0x11330") G1_ALIAS("SceneGame_vtable", "g1_00469460", "0x11370") G1_ALIAS("SceneGame_vtableProgress", "g1_00469460", "0x11388"));
