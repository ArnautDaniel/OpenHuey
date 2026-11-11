
**Read again, 2026-11 (0.2.45)** against subscreen.c / text.c:
- The box's questions as the message window's (Task_StateType / StateChoice): the cursor starts as the choice says
  - "throw away?", "change equipment?", "reset defaults?" on NO; the triangle goes to the last option where
  allowed; up / down round, left / right along a line; the cursor's sound only for a change, the decide's on an
  answer; a plain message closed by Select, L1, R1 or a face button.
- Select closes the menu itself only (SubScreen_StateRun clears the request outside mode 0): not the word plate,
  the save point, the wardrobe, the slot machine, the mini-game's screens, the galleries, the options alone.
- Opening sounds only for the menu, the word plate, the slot machine and the options (SubScreen_Open).
- It opens only with Fiona not a script's and not held (+0xE0, +0x14E8 5: her plight's reach bits 8 / 16), and
  clears the item's tab (SubScreen_TakeOver).
- The options: each change heard or seen at once, the cross keeps it, the triangle - or Select - puts back what
  it was; the sound out round 0 -> 1 -> 2 (heard at once); the volume 1/64 a press; the position's sound each step.
- A key thing refused only with the stalker in the game (told as the world last ran) and not a script's (item_usable).
- The play time goes on with any sub screen up (`menu-frame`: SceneGame_SubSubScreen's PlayTime_Tick), not the pause's.
- Use plays only what came of it; a plate's word named for the plate picked; the item's picture slides in each
  time; the map back on her page as L1 / R1 turn to it (Map_BackToPlayer); the wardrobe's sounds as Costumes_State's
  (opened by R1 / right the cursor's, closed by L1 / left the cursor's, done quietly), his row hidden with a coat
  not of the three; the left stick past half way as the arrows (Pads_BuildInput).
Not built: the controller-type row (the keys are the keyboard's).
