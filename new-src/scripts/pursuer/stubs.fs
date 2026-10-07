\ pursuer/stubs.fs - generated (scratchpad tools/stubs.py): a stub for each of his vtable
\ entries, taking its arguments and giving 0; the ported words replace them.
IN: pursuer.stubs
USING: engine chars pursuer.core ;

:noname  drop s" Debilitas_dtor" (not-yet) 0 ;  $8 vt!
:noname   s" Pursuer_Reset" (not-yet) ;  $C vt!
:noname   s" Pursuer_Cleanup" (not-yet) ;  $10 vt!
:noname   s" Pursuer_LoadFiles" (not-yet) ;  $14 vt!
:noname   s" Pursuer_FilesLoading" (not-yet) ;  $18 vt!
:noname   s" Pursuer_FilesLoaded" (not-yet) ;  $1C vt!
:noname   s" Pursuer_Unload" (not-yet) ;  $20 vt!
:noname   s" Pursuer_RememberPos" (not-yet) ;  $24 vt!
:noname  drop drop drop s" Pursuer_Place" (not-yet) 0 ;  $28 vt!
:noname   s" Pursuer_LightChange" (not-yet) ;  $2C vt!
:noname   s" Debilitas_Update" (not-yet) ;  $30 vt!
:noname  drop s" Pursuer_LeaveScreen" (not-yet) ;  $34 vt!
:noname   s" Pursuer_ShowUp" (not-yet) ;  $38 vt!
:noname   s" Actor_CanAct" (not-yet) 0 ;  $3C vt!
:noname   s" Pursuer_ModelUpdate" (not-yet) ;  $40 vt!
:noname   s" Pursuer_Think" (not-yet) ;  $44 vt!
:noname   s" Pursuer_PlaceModel" (not-yet) ;  $48 vt!
:noname   s" Pursuer_Disable" (not-yet) ;  $4C vt!
:noname   s" Pursuer_Enable" (not-yet) ;  $50 vt!
:noname   s" Pursuer_LoadMessage" (not-yet) 0 ;  $54 vt!
:noname   s" Pursuer_Deactivate" (not-yet) ;  $58 vt!
:noname   s" Pursuer_Activate" (not-yet) ;  $5C vt!
:noname   s" Pursuer_ResetBehaviour" (not-yet) ;  $60 vt!
:noname  drop drop drop s" Pursuer_PlaceInRoom" (not-yet) 0 ;  $64 vt!
:noname  drop drop drop s" Pursuer_EventConcerns" (not-yet) 0 ;  $68 vt!
:noname   s" Pursuer_SaveState" (not-yet) ;  $6C vt!
:noname   s" Pursuer_LoadState" (not-yet) ;  $70 vt!
:noname  drop s" Pursuer_AttackPoint" (not-yet) 0 ;  $74 vt!
:noname   s" Pursuer_LeftBehind" (not-yet) ;  $78 vt!
:noname   s" Pursuer_BackToStance" (not-yet) ;  $7C vt!
:noname   s" Character_RegionFade" (not-yet) ;  $80 vt!
:noname   s" Pursuer_EventState" (not-yet) ;  $84 vt!
:noname   s" Pursuer_EventCommand" (not-yet) ;  $88 vt!
:noname   s" Pursuer_BackToNormal" (not-yet) ;  $8C vt!
:noname   s" Pursuer_EventReset" (not-yet) ;  $90 vt!
:noname  drop s" Character_TimerDown" (not-yet) 0 ;  $94 vt!
:noname   s" Pursuer_IsBusy" (not-yet) 0 ;  $98 vt!
:noname  drop drop s" Debilitas_DoorOffset" (not-yet) ;  $9C vt!
:noname   s" Debilitas_TurnRate" (not-yet) 0e ;  $A0 vt!
:noname   s" Debilitas_TurnRateFast" (not-yet) 0e ;  $A4 vt!
:noname   s" Debilitas_BlockFlags" (not-yet) 0 ;  $A8 vt!
:noname  drop drop drop s" Debilitas_GoTo" (not-yet) ;  $AC vt!
:noname   s" Debilitas_HeadForFiona" (not-yet) ;  $B0 vt!
:noname  drop s" Debilitas_HeadFor" (not-yet) ;  $B4 vt!
:noname  drop s" NPC_HeadNearFiona" (not-yet) ;  $B8 vt!
:noname  drop s" NPC_HeadNearHewie" (not-yet) ;  $BC vt!
:noname   s" NPC_FionaInReach" (not-yet) 0 ;  $C0 vt!
:noname   s" NPC_HewieInReach" (not-yet) 0 ;  $C4 vt!
:noname   s" NPC_HearNoise" (not-yet) 0 ;  $C8 vt!
:noname   s" Pursuer_PickTarget" (not-yet) 0 ;  $CC vt!
:noname   s" Pursuer_LocateTarget" (not-yet) ;  $D0 vt!
:noname  drop drop s" NPC_PathLengthTo" (not-yet) 0e ;  $D4 vt!
:noname   s" NPC_PathLengthGoal" (not-yet) 0 ;  $D8 vt!
:noname   s" NPC_PathLengthSpot" (not-yet) 0 ;  $DC vt!
:noname  drop s" NPC_PathLengthChar" (not-yet) 0 ;  $E0 vt!
:noname   s" NPC_DoorBreak" (not-yet) ;  $E4 vt!
:noname   s" Debilitas_PickDestination" (not-yet) 0 ;  $E8 vt!
:noname  drop s" NPC_CanUseExit" (not-yet) 0 ;  $EC vt!
:noname  drop s" NPC_ExitArg" (not-yet) ;  $F0 vt!
:noname   s" Debilitas_Setup" (not-yet) ;  $F4 vt!
:noname   s" Debilitas_ModelFiles" (not-yet) 0 ;  $F8 vt!
:noname   s" Debilitas_MotionFiles" (not-yet) 0 ;  $FC vt!
:noname   s" Pursuer_Footsteps" (not-yet) ;  $100 vt!
:noname   s" Pursuer_StopSounds" (not-yet) ;  $104 vt!
:noname   s" Pursuer_PathNodeSound" (not-yet) 0 ;  $108 vt!
:noname   s" Pursuer_InStance2Anim" (not-yet) 0 ;  $10C vt!
:noname   s" Pursuer_GrabOrder" (not-yet) 0 ;  $110 vt!
:noname  drop s" Pursuer_StartAction" (not-yet) ;  $114 vt!
:noname  drop s" Pursuer_StartActionNext" (not-yet) ;  $118 vt!
:noname   s" Debilitas_GivesUp" (not-yet) 0 ;  $11C vt!
:noname   s" Pursuer_ModesSearching" (not-yet) ;  $120 vt!
:noname   s" Pursuer_Modes" (not-yet) ;  $124 vt!
:noname   s" Debilitas_StandAnim" (not-yet) ;  $128 vt!
:noname  fdrop s" Pursuer_Threat" (not-yet) ;  $12C vt!
:noname  drop s" Debilitas_AttackTable" (not-yet) ;  $130 vt!
:noname   s" Pursuer_PickAttack" (not-yet) ;  $134 vt!
:noname  drop drop drop s" Pursuer_BonePositions" (not-yet) ;  $138 vt!
:noname   s" Pursuer_PickStep" (not-yet) ;  $13C vt!
:noname   s" Pursuer_PickStepAlt" (not-yet) ;  $140 vt!
:noname  drop s" Pursuer_LeaveScreenEnding" (not-yet) ;  $144 vt!
:noname   s" Pursuer_EnterRoom" (not-yet) ;  $148 vt!
:noname  drop s" Pursuer_EventOver" (not-yet) ;  $14C vt!
:noname   s" Pursuer_EventOver2" (not-yet) ;  $150 vt!
:noname   s" Pursuer_AfterMove" (not-yet) ;  $154 vt!
:noname   s" Pursuer_PlanWayOn" (not-yet) ;  $158 vt!
:noname   s" Pursuer_PlanToGoal" (not-yet) ;  $15C vt!
:noname   s" Pursuer_FollowPlan" (not-yet) ;  $160 vt!
:noname   s" Pursuer_Pace" (not-yet) ;  $164 vt!
:noname   s" Pursuer_TravelOffscreen" (not-yet) ;  $168 vt!
:noname   s" Pursuer_Plan16C" (not-yet) ;  $16C vt!
:noname   s" Pursuer_Plan170" (not-yet) ;  $170 vt!
:noname   s" Pursuer_KnockAtDoor" (not-yet) ;  $174 vt!
:noname   s" Pursuer_WaitRoomFlag" (not-yet) ;  $178 vt!
:noname   s" Pursuer_Move17C" (not-yet) ;  $17C vt!
:noname   s" Pursuer_Move180" (not-yet) ;  $180 vt!
:noname   s" Pursuer_Move184" (not-yet) ;  $184 vt!
:noname   s" Pursuer_BackToStand" (not-yet) ;  $188 vt!
:noname   s" Pursuer_Move18C" (not-yet) ;  $18C vt!
:noname   s" Pursuer_Nothing190" (not-yet) ;  $190 vt!
:noname   s" Pursuer_WalkToGoal" (not-yet) ;  $194 vt!
:noname   s" Pursuer_StepBack" (not-yet) ;  $198 vt!
:noname   s" Pursuer_CloseInGoal" (not-yet) ;  $19C vt!
:noname   s" Pursuer_Chase1A0" (not-yet) ;  $1A0 vt!
:noname   s" Pursuer_Chase1A4" (not-yet) ;  $1A4 vt!
:noname   s" Debilitas_ChaseTarget" (not-yet) ;  $1A8 vt!
:noname   s" Pursuer_DoorLineUp" (not-yet) ;  $1AC vt!
:noname   s" Pursuer_DoorGoThrough" (not-yet) ;  $1B0 vt!
:noname   s" Pursuer_DoorFace" (not-yet) ;  $1B4 vt!
:noname   s" Pursuer_DoorOpen" (not-yet) ;  $1B8 vt!
:noname   s" Pursuer_DoorThrough" (not-yet) ;  $1BC vt!
:noname   s" Pursuer_DoorPush" (not-yet) ;  $1C0 vt!
:noname   s" Pursuer_Door1C4" (not-yet) ;  $1C4 vt!
:noname   s" Pursuer_Door1C8" (not-yet) ;  $1C8 vt!
:noname   s" Pursuer_DoorWalkTo" (not-yet) ;  $1CC vt!
:noname   s" Pursuer_DoorWalking" (not-yet) ;  $1D0 vt!
:noname   s" Pursuer_DoorFacing" (not-yet) ;  $1D4 vt!
:noname   s" Pursuer_DoorBarge" (not-yet) ;  $1D8 vt!
:noname   s" Pursuer_DoorWait" (not-yet) ;  $1DC vt!
:noname   s" Pursuer_DoorIdle" (not-yet) ;  $1E0 vt!
:noname   s" Pursuer_Door1E4" (not-yet) ;  $1E4 vt!
:noname   s" Pursuer_ExitThrough" (not-yet) ;  $1E8 vt!
:noname   s" Pursuer_ExitOpen" (not-yet) ;  $1EC vt!
:noname   s" Pursuer_Exit1F0" (not-yet) ;  $1F0 vt!
:noname   s" Pursuer_ExitGoTo" (not-yet) ;  $1F4 vt!
:noname   s" Pursuer_ExitOpenStep" (not-yet) ;  $1F8 vt!
:noname   s" Pursuer_DoorThroughOpen" (not-yet) ;  $1FC vt!
:noname   s" Debilitas_ExitDone" (not-yet) ;  $200 vt!
:noname   s" Pursuer_DoorBackAway" (not-yet) ;  $204 vt!
:noname   s" Pursuer_Door208" (not-yet) ;  $208 vt!
:noname   s" Pursuer_DoorBackOff" (not-yet) ;  $20C vt!
:noname   s" Pursuer_DoorWalk" (not-yet) ;  $210 vt!
:noname   s" Pursuer_Door214" (not-yet) ;  $214 vt!
:noname   s" Pursuer_DoorPushThrough" (not-yet) ;  $218 vt!
:noname   s" Pursuer_WalkToSpot" (not-yet) ;  $21C vt!
:noname   s" Pursuer_DoorArmOpen" (not-yet) ;  $220 vt!
:noname   s" Debilitas_Stairs" (not-yet) ;  $224 vt!
:noname   s" Debilitas_DoorAnim" (not-yet) ;  $228 vt!
:noname   s" Pursuer_DoorApproach" (not-yet) ;  $22C vt!
:noname   s" Pursuer_WalkAside" (not-yet) ;  $230 vt!
:noname   s" Pursuer_LookAround" (not-yet) ;  $234 vt!
:noname   s" Pursuer_SearchRoute" (not-yet) ;  $238 vt!
:noname   s" Pursuer_ReactToEvent" (not-yet) ;  $23C vt!
:noname   s" Pursuer_StepCount" (not-yet) ;  $240 vt!
:noname   s" Pursuer_FollowPathExit" (not-yet) ;  $244 vt!
:noname   s" Pursuer_Arrived" (not-yet) ;  $248 vt!
:noname   s" Pursuer_WalkToExit" (not-yet) ;  $24C vt!
:noname   s" Pursuer_OnToNextExit" (not-yet) ;  $250 vt!
:noname   s" Pursuer_NextPathExit" (not-yet) ;  $254 vt!
:noname   s" Pursuer_GoToDoor" (not-yet) ;  $258 vt!
:noname   s" Pursuer_Behaviour25C" (not-yet) ;  $25C vt!
:noname   s" Pursuer_BehaviourSearch" (not-yet) ;  $260 vt!
:noname   s" Debilitas_ChaseDecision" (not-yet) ;  $264 vt!
:noname   s" Pursuer_BehaviourStalk" (not-yet) ;  $268 vt!
:noname   s" Pursuer_BehaviourFollow" (not-yet) ;  $26C vt!
:noname   s" Pursuer_GoForHewie" (not-yet) ;  $270 vt!
:noname   s" Pursuer_BehaviourHewie" (not-yet) ;  $274 vt!
:noname   s" Pursuer_ChaseFiona" (not-yet) ;  $278 vt!
:noname   s" Debilitas_BehaviourRun" (not-yet) ;  $27C vt!
:noname   s" Pursuer_GoAfterFiona" (not-yet) ;  $280 vt!
:noname   s" Pursuer_BehaviourDoors" (not-yet) ;  $284 vt!
:noname   s" Pursuer_Behaviour288" (not-yet) ;  $288 vt!
:noname   s" Pursuer_BehaviourAttack" (not-yet) ;  $28C vt!
:noname   s" Pursuer_BehaviourIdle" (not-yet) ;  $290 vt!
:noname   s" Pursuer_OffscreenStep" (not-yet) ;  $294 vt!
:noname   s" Pursuer_OffscreenUpdate" (not-yet) ;  $298 vt!
:noname   s" Pursuer_BehaviourEnded" (not-yet) ;  $29C vt!
:noname   s" Pursuer_FrameUpdate" (not-yet) ;  $2A0 vt!
:noname   s" Pursuer_ClearBehaviour" (not-yet) ;  $2A4 vt!
:noname   s" Pursuer_GiveUp" (not-yet) ;  $2A8 vt!
:noname   s" Debilitas_GoForFiona" (not-yet) ;  $2AC vt!
:noname   s" Debilitas_HeadingStep" (not-yet) ;  $2B0 vt!
:noname   s" Debilitas_FreshStart" (not-yet) ;  $2B4 vt!
:noname   s" Debilitas_CarryOn" (not-yet) ;  $2B8 vt!
:noname   s" Pursuer_Caught" (not-yet) ;  $2BC vt!
:noname   s" Debilitas_Timer10s" (not-yet) ;  $2C0 vt!
:noname   s" Pursuer_Timer15s" (not-yet) ;  $2C4 vt!
:noname  drop s" Debilitas_SetTimer" (not-yet) ;  $2C8 vt!
:noname   s" Pursuer_NoGiveUp" (not-yet) ;  $2CC vt!
:noname   s" Pursuer_Get2D0" (not-yet) 0 ;  $2D0 vt!
:noname   s" Pursuer_Get2D4" (not-yet) 0 ;  $2D4 vt!
:noname  drop drop s" Debilitas_ActionOffsets" (not-yet) ;  $2D8 vt!
:noname   s" Debilitas_LookFrames" (not-yet) 0e ;  $2DC vt!
:noname   s" Debilitas_LookSwing" (not-yet) 0e ;  $2E0 vt!
:noname   s" Debilitas_ReachFiona" (not-yet) 0e ;  $2E4 vt!
:noname   s" Debilitas_Dist2E8" (not-yet) 0e ;  $2E8 vt!
:noname   s" Debilitas_AttackAngle" (not-yet) 0e ;  $2EC vt!
:noname   s" Debilitas_ReachHewie" (not-yet) 0e ;  $2F0 vt!
:noname   s" Debilitas_AttackRange" (not-yet) 0e ;  $2F4 vt!
:noname   s" Debilitas_SpeedTop" (not-yet) 0e ;  $2F8 vt!
:noname   s" Debilitas_SpeedBase" (not-yet) 0e ;  $2FC vt!
:noname   s" Pursuer_FrightSeen" (not-yet) 0e ;  $300 vt!
:noname   s" Pursuer_FrightAttack" (not-yet) 0e ;  $304 vt!
:noname   s" Pursuer_ThreatAmount" (not-yet) 0e ;  $308 vt!
:noname   s" Debilitas_AttackAnimA" (not-yet) 0 ;  $30C vt!
:noname   s" Debilitas_AttackAnimB" (not-yet) 0 ;  $310 vt!
:noname   s" Debilitas_RoomSpots" (not-yet) 0 ;  $314 vt!
:noname   s" Pursuer_Get318" (not-yet) 0 ;  $318 vt!
:noname  drop s" Pursuer_SetRage" (not-yet) ;  $31C vt!
:noname   s" Pursuer_WalkAnim" (not-yet) 0 ;  $320 vt!
:noname   s" Pursuer_StanceAnim" (not-yet) 0 ;  $324 vt!
:noname   s" Debilitas_SlowWalkAnim" (not-yet) 0 ;  $328 vt!
