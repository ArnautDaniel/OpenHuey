/* Generated (tools/gen_getters.py): the per-room classes' and other trivial getters -
 * the address of a static table (room event scripts etc.), one entry of a table, or a call
 * through a member-function-pointer table. */
#include "common.h"
#include "ptmf.h"

extern u8 D_003C5D90[];
extern u8 D_003C6348[];
extern u8 D_003C6450[];
extern u8 D_003D5B80[];
extern u8 D_003D73B0[];
extern u8 D_003E5268[];
extern u8 D_003E5270[];
extern u8 D_003E88D0[];
extern u8 D_003E9F18[];
extern u8 D_003ED800[];
extern u8 D_003ED960[];
extern u8 D_003ED9E0[];
extern u8 D_003EDBC0[];
extern u8 D_003EDC40[];
extern void *D_003EE6C0[];
extern void *D_003EE740[];
extern u8 D_003EE760[];
extern u8 D_003EE770[];
extern u8 D_003EE820[];
extern u8 D_003EE920[];
extern u8 D_003EEB90[];
extern u8 D_003EEBC0[];
extern u8 D_003EEC00[];
extern void *D_003EEDC0[];
extern u8 D_003EEDD8[];
extern u8 D_003EEDF0[];
extern u8 D_003EEF70[];
extern u8 D_003EF0D0[];
extern u8 D_003EF5B0[];
extern u8 D_003EF780[];
extern u8 D_003EF8E0[];
extern void *D_003F0310[];
extern void *D_003F03E0[];
extern u8 D_003F0410[];
extern u8 D_003F0430[];
extern u8 D_003F0450[];
extern u8 D_003F0540[];
extern u8 D_003F0580[];
extern u8 D_003F06A0[];
extern u8 D_003F0820[];
extern u8 D_003F0860[];
extern void *D_003F0D60[];
extern void *D_003F0DB0[];
extern u8 D_003F0DD0[];
extern u8 D_003F0DF0[];
extern u8 D_003F0E80[];
extern u8 D_003F0F70[];
extern u8 D_003F1060[];
extern void *D_003F1740[];
extern void *D_003F17B0[];
extern u8 D_003F17F0[];
extern u8 D_003F1800[];
extern u8 D_003F1840[];
extern u8 D_003F1930[];
extern u8 D_003F1A30[];
extern u8 D_003F1A50[];
extern void *D_003F1B40[];
extern u8 D_003F1B60[];
extern u8 D_003F1BD0[];
extern u8 D_003F1CD0[];
extern u8 D_003F1D58[];
extern u8 D_003F1D70[];
extern u8 D_003F1F00[];
extern void *D_003F2170[];
extern void *D_003F21D0[];
extern u8 D_003F21E0[];
extern u8 D_003F21F0[];
extern u8 D_003F22D0[];
extern u8 D_003F24B0[];
extern u8 D_003F24F0[];
extern u8 D_003F2500[];
extern u8 D_003F2640[];
extern u8 D_003F2780[];
extern u8 D_003F28A0[];
extern u8 D_003F2910[];
extern u8 D_003F2980[];
extern void *D_003F3180[];
extern void *D_003F3200[];
extern u8 D_003F3230[];
extern u8 D_003F3240[];
extern u8 D_003F32D0[];
extern u8 D_003F3330[];
extern u8 D_003F3400[];
extern u8 D_003F34A0[];
extern u8 D_003F34D8[];
extern void *D_003F3C00[];
extern void *D_003F3C60[];
extern u8 D_003F3C80[];
extern u8 D_003F3C90[];
extern u8 D_003F3CF0[];
extern u8 D_003F3DA0[];
extern u8 D_003F3E90[];
extern u8 D_003F3EF0[];
extern void *D_003F4350[];
extern void *D_003F4398[];
extern u8 D_003F43B0[];
extern u8 D_003F43C0[];
extern u8 D_003F43E0[];
extern u8 D_003F4470[];
extern void *D_003F4620[];
extern u8 D_003F4650[];
extern u8 D_003F46E0[];
extern u8 D_003F4720[];
extern u8 D_003F4850[];
extern void *D_003F53B0[];
extern void *D_003F5440[];
extern u8 D_003F5490[];
extern u8 D_003F54D0[];
extern u8 D_003F5540[];
extern u8 D_003F5610[];
extern u8 D_003F5710[];
extern u8 D_003F57E0[];
extern u8 D_003F5980[];
extern void *D_003F5E80[];
extern u8 D_003F5EA0[];
extern u8 D_003F5EC0[];
extern u8 D_003F5FC0[];
extern u8 D_003F6120[];
extern u8 D_003F6420[];
extern void *D_003F6E70[];
extern void *D_003F6F40[];
extern u8 D_003F6F68[];
extern u8 D_003F6F80[];
extern u8 D_003F7060[];
extern u8 D_003F7150[];
extern u8 D_003F72D0[];
extern u8 D_003F73B0[];
extern u8 D_003F74E0[];
extern void *D_003F78D0[];
extern u8 D_003F7910[];
extern u8 D_003F7920[];
extern u8 D_003F7A20[];
extern u8 D_003F7AC0[];
extern u8 D_003F7E00[];
extern void *D_003F81D0[];
extern u8 D_003F81F0[];
extern u8 D_003F8200[];
extern u8 D_003F8350[];
extern u8 D_003F83F0[];
extern u8 D_003F8750[];
extern u8 D_003F8900[];
extern u8 D_003F89D0[];
extern void *D_003F90F0[];
extern void *D_003F9170[];
extern u8 D_003F9190[];
extern u8 D_003F91A0[];
extern u8 D_003F92E0[];
extern u8 D_003F93A0[];
extern u8 D_003F9530[];
extern u8 D_003F95F0[];
extern void *D_003F9980[];
extern void *D_003F99B0[];
extern u8 D_003F99C0[];
extern u8 D_003F99E0[];
extern u8 D_003F9A30[];
extern u8 D_003F9AC0[];
extern u8 D_003F9B60[];
extern void *D_003FA030[];
extern void *D_003FA070[];
extern u8 D_003FA080[];
extern u8 D_003FA090[];
extern u8 D_003FA110[];
extern u8 D_003FA1C0[];
extern u8 D_003FA2C0[];
extern u8 D_003FA330[];
extern void *D_003FA700[];
extern void *D_003FA750[];
extern u8 D_003FA770[];
extern u8 D_003FA780[];
extern u8 D_003FA7C0[];
extern u8 D_003FA800[];
extern u8 D_003FA820[];
extern u8 D_003FA850[];
extern u8 D_003FAA08[];
extern u8 D_003FAA20[];
extern u8 D_003FAAA0[];
extern u8 D_003FAB30[];
extern u8 D_003FACB0[];
extern u8 D_003FAD40[];
extern void *D_003FB320[];
extern void *D_003FB370[];
extern u8 D_003FB390[];
extern u8 D_003FB3B0[];
extern u8 D_003FB420[];
extern u8 D_003FB520[];
extern u8 D_003FB720[];
extern u8 D_003FB840[];
extern void *D_003FBFD0[];
extern void *D_003FC020[];
extern u8 D_003FC040[];
extern u8 D_003FC060[];
extern u8 D_003FC0A0[];
extern u8 D_003FC110[];
extern u8 D_003FC220[];
extern u8 D_003FC290[];
extern void *D_003FC630[];
extern void *D_003FC678[];
extern u8 D_003FC690[];
extern u8 D_003FC6A0[];
extern u8 D_003FC6F0[];
extern u8 D_003FC770[];
extern u8 D_003FC8F0[];
extern void *D_003FCAD0[];
extern void *D_003FCB00[];
extern u8 D_003FCB18[];
extern u8 D_003FCB30[];
extern u8 D_003FCBC0[];
extern u8 D_003FCC40[];
extern u8 D_003FCD70[];
extern u8 D_004070F0[];
extern u8 D_00407300[];
extern u8 D_00407340[];
extern u8 D_00407430[];
extern void *D_00407A80[];
extern u8 D_00407AC0[];
extern u8 D_0040AC70[];
extern u8 D_0040AD70[];
extern u8 D_0040AE80[];
extern u8 D_00412410[];
extern u8 D_00412430[];
extern u8 D_00412510[];
extern u8 D_004125E0[];
extern u8 D_00412688[];
extern u8 D_00415BA0[];
extern u8 D_00415C90[];
extern u8 D_00415DB0[];
extern u8 D_00415FA0[];
extern u8 D_00416030[];
extern u8 D_00416130[];
extern void *D_004164C0[];
extern u8 D_004164D0[];
extern u8 D_00417190[];
extern u8 D_00417290[];
extern u8 D_00417310[];
extern u8 D_00417320[];
extern u8 D_00417420[];
extern u8 D_004174A0[];
extern u8 D_00419DD0[];
extern u8 D_00419E10[];
extern u8 D_0041A5F0[];
extern u8 D_0041B020[];
extern u8 D_0041B080[];
extern u8 D_0041B100[];
extern u8 D_0041B120[];
extern u8 D_0041B130[];
extern u8 D_0041B1B0[];
extern u8 D_0041B270[];
extern u8 D_0041B370[];
extern u8 D_0041B450[];
extern u8 D_0041B560[];
extern u8 D_0041B5F0[];
extern u8 D_0041BE40[];
extern u8 D_0041C390[];
extern u8 D_0041C3E0[];
extern u8 D_0041C420[];
extern void *D_0041CA40[];
extern u8 D_0041CA90[];
extern u8 D_0041CB20[];
extern u8 D_0041CC00[];
extern u8 D_0041CD90[];
extern u8 D_0041CDF0[];
extern u8 D_0041CEE8[];
extern void *D_0041D100[];
extern u8 D_0041D120[];
extern u8 D_0041D140[];
extern u8 D_0041D1C0[];
extern u8 D_0041D2E0[];
extern u8 D_0041D330[];
extern u8 D_0041D3C0[];
extern u8 D_0041D4E0[];
extern void *D_0041D7A0[];
extern void *D_0041D808[];
extern u8 D_0041DD20[];
extern u8 D_0041DD90[];
extern u8 D_0041DE10[];
extern u8 D_0041DF60[];
extern u8 D_0041DF70[];
extern void *D_0041E0E0[];
extern u8 D_0041E0F0[];
extern u8 D_0041E110[];
extern u8 D_0041E260[];
extern u8 D_0041E360[];
extern u8 D_0041E700[];
extern void *D_0041F4E0[];
extern u8 D_0041F5C0[];
extern u8 D_004295A0[];
extern u8 D_00429620[];
extern u8 D_00429640[];
extern u8 D_00429700[];
extern u8 D_004297C0[];
extern u8 D_00429800[];
extern u8 D_0042C870[];
extern u8 D_0042C8B0[];
extern u8 D_004314B0[];
extern u8 D_004315E0[];
extern u8 D_00431750[];
extern u8 D_00431A60[];
extern void *D_00431DB0[];
extern u8 D_00431DD0[];
extern u8 D_00444170[];
extern u8 D_004441C0[];
extern u8 D_00444200[];
extern u8 D_00444270[];
extern void *D_00444360[];
extern u8 D_00444390[];
extern u8 D_004443C0[];
extern u8 D_00444400[];
extern u8 D_00444460[];
extern void *D_004444C0[];
extern u8 D_004444E0[];
extern u8 D_00444530[];
extern u8 D_00444570[];
extern u8 D_004445D0[];
extern void *D_004446B0[];
extern u8 D_004446E0[];
extern u8 D_00444710[];
extern u8 D_00444750[];
extern u8 D_004447C0[];
extern void *D_00444830[];
extern u8 D_00444B10[];
extern u8 D_00445BC0[];
extern u8 D_00445C20[];
extern u8 D_00445C70[];
extern u8 D_00445C90[];
extern u8 D_00445D70[];
extern void *D_00446960[];
extern u8 D_004469C0[];
extern u8 D_00446A00[];
extern u8 D_00446A80[];
extern u8 D_00446B00[];
extern u8 D_00446BE0[];
extern u8 D_00446C00[];
extern u8 D_00446C10[];
extern u8 D_00446C90[];
extern u8 D_00446CF0[];
extern u8 D_00446D40[];
extern u8 D_00446D60[];
extern u8 D_00446E60[];
extern u8 D_00446EE0[];
extern u8 D_00446F20[];
extern u8 D_00446F30[];
extern u8 D_00447030[];
extern u8 D_004470D0[];
extern u8 D_00447110[];
extern u8 D_00447130[];
extern u8 D_00447230[];
extern u8 D_004472B0[];
extern u8 D_004472F0[];
extern u8 D_00447300[];
extern u8 D_00447330[];
extern u8 D_00447460[];
extern u8 D_00447550[];
extern u8 D_00447560[];
extern void *D_00447608[];
extern u8 D_00447660[];
extern u8 D_00447680[];
extern u8 D_00447790[];
extern u8 D_00447890[];
extern u8 D_004478B0[];
extern void *D_00447A10[];
extern void *D_00447A50[];
extern u8 D_00447A60[];
extern u8 D_00447A70[];
extern u8 D_00447A90[];
extern u8 D_00447AD0[];
extern u8 D_00447B00[];
extern u8 D_00447BC0[];
extern u8 D_00447BD0[];
extern u8 D_00447BF0[];
extern u8 D_00447CF0[];
extern u8 D_00447DA0[];
extern u8 D_00447E60[];
extern u8 D_00447E80[];
extern u8 D_00447E90[];
extern u8 D_00447F40[];
extern u8 D_00447F60[];
extern u8 D_00447FC8[];
extern u8 D_004480B0[];
extern u8 D_004480F0[];
extern u8 D_00448200[];
extern u8 D_00448320[];
extern u8 D_00448340[];
extern void *D_004484E0[];
extern u8 D_00448570[];
extern u8 D_00448590[];
extern u8 D_004485E0[];
extern u8 D_00448660[];
extern u8 D_00448740[];
extern u8 D_004488A0[];
extern u8 D_004488B0[];
extern u8 D_00448990[];
extern u8 D_00448A10[];
extern u8 D_00448CD0[];
extern u8 D_00448D10[];
extern u8 D_00448D20[];
extern u8 D_00448DA0[];
extern u8 D_00448E30[];
extern u8 D_00448F30[];
extern u8 D_00449010[];
extern void *D_00449210[];
extern u8 D_00449240[];
extern u8 D_00449250[];
extern u8 D_004492C0[];
extern u8 D_00449380[];
extern u8 D_004494F0[];
extern void *D_00449540[];
extern u8 D_00449558[];
extern u8 D_00449570[];
extern u8 D_00449590[];
extern u8 D_004495E0[];
extern u8 D_00449640[];
extern u8 D_00449660[];
extern void *D_00449890[];
extern u8 D_004498E0[];
extern u8 D_004498F0[];
extern u8 D_00449910[];
extern u8 D_00449990[];
extern u8 D_004499E0[];
extern u8 D_00449AC0[];
extern u8 D_00449AD0[];
extern u8 D_00449B20[];
extern u8 D_00449BB0[];
extern u8 D_00449CE0[];
extern u8 D_00449D00[];
extern void *D_00449E90[];
extern u8 D_00449EC0[];
extern u8 D_00449EE0[];
extern u8 D_00449F60[];
extern u8 D_00449FB0[];
extern u8 D_0044A2A0[];
extern u8 D_0044A2C0[];
extern u8 D_0044A380[];
extern u8 D_0044A3C0[];
extern u8 D_0044A470[];
extern void *D_0044A928[];
extern u8 D_0044A990[];
extern u8 D_0044C898[];
extern u8 D_0044C900[];
extern u8 D_0044CAC0[];
extern u8 D_0044DAB0[];
extern u8 D_00451318[];
extern u8 D_004555C8[];
extern u8 D_00455870[];
extern u8 D_00455C38[];
extern u8 D_00455DD8[];
extern u8 D_00455E18[];
extern u8 D_00456EB0[];
extern u8 D_00456F70[];
extern u8 D_004574B8[];
extern u8 D_00457558[];
extern u8 D_004575C0[];
extern u8 D_00457ED0[];
extern u8 D_00459930[];
extern u8 D_00459F60[];
extern u8 D_0045A4D8[];
extern u8 D_0045A6E0[];
extern u8 D_0045E4C0[];
extern u8 D_0045E4E0[];
extern u8 D_00463320[];
extern u8 D_00463340[];
extern u8 D_00463A50[];
extern void *D_0047A9A8[];
extern void *D_0047AA38[];
extern u8 D_0047AB90[];
extern void *D_0047ABF0[];
extern u8 D_0047AC50[];
extern u8 D_0047AC58[];
extern void *D_0047ACA0[];
extern u8 D_0047AD44[];
extern u8 D_0047AD48[];
extern u8 D_0047AD50[];
extern u8 D_0047AD58[];
extern u8 D_0047E878[];
extern u8 D_01989558[];
extern PTMF D_01990700[];
extern PTMF D_01990718[];
extern PTMF D_01990730[];
extern PTMF D_01990760[];
extern PTMF D_01990780[];
extern PTMF D_019907A0[];
extern PTMF D_019907D0[];
extern PTMF D_019907E0[];
extern PTMF D_01990800[];
extern PTMF D_01990818[];
extern PTMF D_01990830[];
extern PTMF D_01990848[];
extern PTMF D_01990860[];
extern PTMF D_01990890[];
extern PTMF D_019908D0[];
extern PTMF D_019908E0[];
extern PTMF D_019908F0[];
extern PTMF D_01990900[];
extern PTMF D_01990910[];
extern PTMF D_01990920[];
extern PTMF D_01990930[];
extern PTMF D_01990940[];
extern PTMF D_01990950[];
extern PTMF D_01990960[];
extern PTMF D_01990978[];
extern PTMF D_01990990[];
extern PTMF D_019909B0[];
extern PTMF D_019909C8[];
extern PTMF D_019909D8[];
extern PTMF D_019909F0[];
extern PTMF D_01990A40[];
extern PTMF D_01990A70[];
extern PTMF D_01990AC8[];
extern PTMF D_01990AD8[];
extern PTMF D_01990AF0[];
extern PTMF D_01990B10[];
extern PTMF D_01990B38[];
extern PTMF D_01990B48[];
extern PTMF D_01990B58[];
extern PTMF D_01990B70[];
extern PTMF D_01990B98[];
extern PTMF D_01990BB0[];
extern PTMF D_01990C00[];
extern PTMF D_01990C10[];
extern PTMF D_01990C38[];
extern PTMF D_01990C48[];
extern PTMF D_01990C58[];
extern PTMF D_01990C68[];
extern PTMF D_01990C80[];
extern PTMF D_01990CB0[];
extern PTMF D_01990CC8[];
extern PTMF D_01990CE0[];
extern PTMF D_01990D10[];
extern PTMF D_01990D40[];
extern PTMF D_01990D70[];
extern PTMF D_01990D80[];
extern PTMF D_01990D90[];
extern PTMF D_01990DA8[];
extern PTMF D_01990DB8[];
extern PTMF D_01990DC8[];
extern PTMF D_01990DD8[];
extern PTMF D_01990DE8[];
extern PTMF D_01990DF8[];
extern PTMF D_01990E08[];
extern PTMF D_01990E20[];
extern PTMF D_01990E38[];
extern PTMF D_01990E48[];
extern PTMF D_01990E78[];
extern PTMF D_01990E88[];
extern PTMF D_01990E98[];
extern PTMF D_01990EA8[];
extern PTMF D_01990EB8[];
extern PTMF D_01990EC8[];
extern PTMF D_01990ED8[];
extern PTMF D_01990EE8[];
extern PTMF D_01990F00[];
extern PTMF D_01990F18[];
extern PTMF D_01990F30[];
extern PTMF D_01990F48[];
extern PTMF D_01990F58[];
extern PTMF D_01990F68[];
extern PTMF D_01990F78[];
extern PTMF D_01990FD8[];
extern PTMF D_01990FF0[];
extern PTMF D_01991030[];
extern PTMF D_01991070[];
extern PTMF D_01991088[];
extern PTMF D_01991098[];
extern PTMF D_019910B0[];
extern PTMF D_01991140[];
extern PTMF D_01991188[];
extern PTMF D_019911A0[];
extern PTMF D_019911C0[];
extern PTMF D_01991550[];
extern PTMF D_01991570[];
extern PTMF D_019915A0[];
extern PTMF D_019915E8[];
extern PTMF D_01991610[];
extern PTMF D_01991650[];
extern PTMF D_01991660[];
extern PTMF D_019916C0[];
extern PTMF D_019916D8[];
extern PTMF D_019916E8[];
extern PTMF D_019916F8[];
extern PTMF D_01991708[];
extern PTMF D_01991720[];
extern PTMF D_01991760[];
extern PTMF D_01991790[];
extern PTMF D_019917B0[];
extern PTMF D_019917E0[];
extern PTMF D_019917F0[];
extern PTMF D_01991820[];
extern PTMF D_019918C8[];
extern PTMF D_019918D8[];
extern PTMF D_019918F0[];
extern PTMF D_01991910[];
extern PTMF D_01991930[];
extern PTMF D_01991948[];
extern PTMF D_01991958[];
extern PTMF D_01991968[];
extern PTMF D_01991978[];
extern PTMF D_01991990[];
extern PTMF D_019919A8[];
extern PTMF D_019919C0[];
extern PTMF D_019919E8[];
extern PTMF D_019919F8[];
extern PTMF D_01991A08[];
extern PTMF D_01991A18[];
extern PTMF D_01991A28[];
extern PTMF D_01991A38[];
extern PTMF D_01991A48[];
extern PTMF D_01991A58[];
extern PTMF D_01991A68[];
extern PTMF D_01991A78[];
extern PTMF D_01991A88[];
extern PTMF D_01991AB0[];
extern PTMF D_01991AC0[];
extern PTMF D_01991AE8[];
extern PTMF D_01991B00[];
extern PTMF D_01991B18[];
extern PTMF D_01991B28[];
extern PTMF D_01991B38[];
extern PTMF D_01991B48[];
extern PTMF D_01991B58[];
extern PTMF D_01991B68[];
extern PTMF D_01991B78[];
extern PTMF D_01991B90[];
extern PTMF D_01991BB8[];
extern PTMF D_01991BD0[];
extern PTMF D_01991BE8[];
extern PTMF D_01991C00[];
extern PTMF D_01991C20[];
extern PTMF D_01991C50[];
extern PTMF D_01991C70[];
extern PTMF D_01991C90[];
extern PTMF D_01991CB8[];
extern PTMF D_01991CD0[];
extern PTMF D_01991D18[];
extern PTMF D_01991D28[];
extern PTMF D_01991D38[];
extern PTMF D_01991D48[];
extern PTMF D_01991D58[];
extern PTMF D_01991D68[];
extern PTMF D_01991D78[];
extern PTMF D_01991D90[];
extern PTMF D_01991DA8[];
extern PTMF D_01991DC0[];
extern PTMF D_01991DD8[];
extern PTMF D_01991DF0[];
extern PTMF D_01991E08[];
extern PTMF D_01991E18[];
extern PTMF D_01991E28[];
extern PTMF D_01991E38[];
extern PTMF D_01991E48[];
extern PTMF D_01991E60[];
extern PTMF D_01991E90[];

void *func_00100540(void) {
    return D_0044C898;
}

void *func_00100650(void) {
    return D_0044C900;
}

void *func_00102310(void) {
    return D_0044CAC0;
}

void *func_00114B90(void) {
    return D_0044DAB0;
}

void *func_001795E0(void) {
    return D_004297C0;
}

void *func_001795F0(void) {
    return D_00429800;
}

void *func_00179A90(void) {
    return D_0042C870;
}

void *func_00179AA0(void) {
    return D_0042C8B0;
}

/* (possibly dead code: nothing in the game references it) */
void *func_001CEC50(void) {
    return D_00451318;
}

void *func_001D6E40(void) {
    return D_0047E878;
}

/* (possibly dead code: nothing in the game references it) */
void *func_001DC580(void) {
    return D_003C5D90;
}

/* (possibly dead code: nothing in the game references it) */
void *func_001DE030(void) {
    return D_003C6348;
}

/* (possibly dead code: nothing in the game references it) */
void *func_001DE0A8(void) {
    return D_003C6450;
}

void *func_001E61D0(void) {
    return D_004555C8;
}

void *func_001E8138(void) {
    return D_00455870;
}

void *func_001EB650(void) {
    return D_00455C38;
}

void *func_001ED190(void) {
    return D_00455DD8;
}

void *func_001ED1D0(void) {
    return D_003D5B80;
}

void *func_001EDFF0(void) {
    return D_00455E18;
}

void *func_0020B0F0(void) {
    return D_0047AC58;
}

void *func_0020B100(void) {
    return D_00417320;
}

void *func_0020B110(void) {
    return D_00417420;
}

void *func_0020B120(void) {
    return D_004174A0;
}

void *func_0020B190(void) {
    return D_0047AC50;
}

void *func_0020B1A0(void) {
    return D_00417190;
}

void *func_0020B1B0(void) {
    return D_00417290;
}

void *func_0020B1C0(void) {
    return D_00417310;
}

void *func_0020B230(void) {
    return D_004314B0;
}

void *func_0020B240(void) {
    return D_004315E0;
}

void *func_0020B250(void) {
    return D_00431750;
}

void *func_0020B260(void) {
    return D_00431A60;
}

void *func_0020B270(void *self, s32 i) {
    return D_00431DB0[i];
}

void *func_0020B290(void) {
    return D_00431DD0;
}

void *func_0020B300(void) {
    return D_0041DD20;
}

void *func_0020B310(void) {
    return D_0041DD90;
}

void *func_0020B320(void) {
    return D_0041DE10;
}

void *func_0020B330(void) {
    return D_0041DF70;
}

void *func_0020B340(void) {
    return D_0041DF60;
}

void *func_0020B350(void *self, s32 i) {
    return D_0041E0E0[i];
}

void *func_0020B370(void) {
    return D_0041E0F0;
}

void *func_0020B3E0(void) {
    return D_0047AD50;
}

void *func_0020B3F0(void) {
    return D_00429640;
}

void *func_0020B400(void) {
    return D_00429700;
}

void *func_0020B410(void) {
    return D_0047AD58;
}

void *func_0020B480(void) {
    return D_0047AD44;
}

void *func_0020B490(void) {
    return D_004295A0;
}

void *func_0020B4A0(void) {
    return D_00429620;
}

void *func_0020B4B0(void) {
    return D_0047AD48;
}

void *func_0020B520(void) {
    return D_0041B130;
}

void *func_0020B530(void) {
    return D_0041B1B0;
}

void *func_0020B540(void) {
    return D_0041B270;
}

void *func_0020B550(void) {
    return D_0041B370;
}

void *func_0020B560(void) {
    return D_0041B450;
}

void *func_0020B570(void *self, s32 i) {
    return D_0047ACA0[i];
}

void *func_0020B590(void) {
    return D_0041B560;
}

void *func_0020B600(void) {
    return D_00412410;
}

void *func_0020B610(void) {
    return D_00412430;
}

void *func_0020B620(void) {
    return D_00412510;
}

void *func_0020B630(void) {
    return D_004125E0;
}

void *func_0020B640(void *self, s32 i) {
    return D_0047ABF0[i];
}

void *func_0020B660(void) {
    return D_00412688;
}

void *func_0020B6D0(void) {
    return D_0041B020;
}

void *func_0020B6E0(void) {
    return D_0041B080;
}

void *func_0020B6F0(void) {
    return D_0041B100;
}

void *func_0020B700(void) {
    return D_0041B120;
}

void *func_0020B770(void) {
    return D_0047AB90;
}

void *func_0020B780(void) {
    return D_0040AC70;
}

void *func_0020B790(void) {
    return D_0040AD70;
}

void *func_0020B7A0(void) {
    return D_0040AE80;
}

void *func_0020B810(void) {
    return D_00415BA0;
}

void *func_0020B820(void) {
    return D_00415C90;
}

void *func_0020B830(void) {
    return D_00415DB0;
}

void *func_0020B840(void) {
    return D_00415FA0;
}

void *func_0020B850(void) {
    return D_00416030;
}

void *func_0020B860(void) {
    return D_00416130;
}

void *func_0020B870(void *self, s32 i) {
    return D_004164C0[i];
}

void *func_0020B890(void) {
    return D_004164D0;
}

void *func_0020B900(void) {
    return D_004070F0;
}

void *func_0020B910(void) {
    return D_00407300;
}

void *func_0020B920(void) {
    return D_00407340;
}

void *func_0020B930(void) {
    return D_00407430;
}

void *func_0020B940(void *self, s32 i) {
    return D_00407A80[i];
}

void *func_0020B960(void) {
    return D_00407AC0;
}

void *func_0020B9D0(void) {
    return D_0041CA90;
}

void *func_0020B9E0(void) {
    return D_0041CB20;
}

void *func_0020B9F0(void) {
    return D_0041CC00;
}

void *func_0020BA00(void) {
    return D_0041CD90;
}

void *func_0020BA10(void) {
    return D_0041CDF0;
}

void *func_0020BA20(void) {
    return D_0041CEE8;
}

void *func_0020BA30(void *self, s32 i) {
    return D_0041D100[i];
}

void *func_0020BA50(void) {
    return D_0041D120;
}

void *func_0020BAC0(void) {
    return D_003FA780;
}

void *func_0020BAD0(void) {
    return D_003FA7C0;
}

void *func_0020BAE0(void) {
    return D_003FA800;
}

void *func_0020BAF0(void) {
    return D_003FA820;
}

void *func_0020BB00(void) {
    return D_003FA850;
}

void *func_0020BB10(void *self, s32 i) {
    return D_0047AA38[i];
}

void *func_0020BB30(void) {
    return D_003FAA08;
}

void *func_0020BBA0(void) {
    return D_003F21E0;
}

void *func_0020BBB0(void) {
    return D_003F21F0;
}

void *func_0020BBC0(void) {
    return D_003F22D0;
}

void *func_0020BBD0(void) {
    return D_003F24B0;
}

void *func_0020BBE0(void *self, s32 i) {
    return D_0047A9A8[i];
}

void *func_0020BC00(void) {
    return D_003F24F0;
}

void *func_0020BC70(void) {
    return D_003F1800;
}

void *func_0020BC80(void) {
    return D_003F1840;
}

void *func_0020BC90(void) {
    return D_003F1930;
}

void *func_0020BCA0(void) {
    return D_003F1A30;
}

void *func_0020BCB0(void) {
    return D_003F1A50;
}

void *func_0020BCC0(void *self, s32 i) {
    return D_003F1B40[i];
}

void *func_0020BD50(void) {
    return D_003EE770;
}

void *func_0020BD60(void) {
    return D_003EE820;
}

void *func_0020BD70(void) {
    return D_003EE920;
}

void *func_0020BD80(void) {
    return D_003EEB90;
}

void *func_0020BD90(void) {
    return D_003EEBC0;
}

void *func_0020BDA0(void) {
    return D_003EEC00;
}

void *func_0020BDB0(void *self, s32 i) {
    return D_003EEDC0[i];
}

void *func_0020BDD0(void) {
    return D_003EEDD8;
}

void *func_0020BF70(void) {
    return D_00456F70;
}

void *func_0020C110(void) {
    return D_00456EB0;
}

void *func_0020C4C0(void) {
    return D_003D73B0;
}

void *func_00226810(void) {
    return D_003E5268;
}

void *func_0022A6F8(void) {
    return D_003E5270;
}

void *func_00230B48(void) {
    return D_004574B8;
}

/* (possibly dead code: nothing in the game references it) */
void *func_00233720(void) {
    return D_00457558;
}

/* (possibly dead code: nothing in the game references it) */
void *func_00236B70(void) {
    return D_004575C0;
}

/* (possibly dead code: nothing in the game references it) */
void *func_0023AA68(void) {
    return D_00457ED0;
}

void *func_0023AA78(void) {
    return D_003E88D0;
}

void *func_0023FFA0(void) {
    return D_003E9F18;
}

/* (possibly dead code: nothing in the game references it) */
void *func_00246A80(void) {
    return D_00459930;
}

/* (possibly dead code: nothing in the game references it) */
void *func_002577D8(void) {
    return D_00459F60;
}

/* (possibly dead code: nothing in the game references it) */
void *func_00259EE8(void) {
    return D_0045A4D8;
}

/* (possibly dead code: nothing in the game references it) */
void *func_0025B828(void) {
    return D_0045A6E0;
}

/* (possibly dead code: nothing in the game references it) */
void *func_0026EFF0(void) {
    return D_01989558;
}

void *func_002A89E0(void) {
    return D_003ED800;
}

void *func_002A89F0(void) {
    return D_003ED960;
}

void *func_002A8A00(void) {
    return D_003ED9E0;
}

void *func_002A8A10(void) {
    return D_003EDBC0;
}

void *func_002A8A20(void) {
    return D_003EDC40;
}

void *func_002A8A30(void *self, s32 i) {
    return D_003EE6C0[i];
}

void *func_002A8A50(void) {
    return D_003EE760;
}

void *func_002A8A60(void *self, s32 i) {
    return D_003EE740[i];
}

/* (self->*D_01990718[i])(a, b) */
s32 func_002A8A80(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990718[i & 0xFF], a, b);
}

/* (self->*D_01990700[i])(a, b) */
s32 func_002A8AC0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990700[i & 0xFF], a, b);
}

void *func_002A8E60(void) {
    return D_003EEDF0;
}

void *func_002A8E70(void) {
    return D_003EEF70;
}

void *func_002A8E80(void) {
    return D_003EF0D0;
}

void *func_002A8E90(void) {
    return D_003EF5B0;
}

void *func_002A8EA0(void) {
    return D_003EF780;
}

void *func_002A8EB0(void) {
    return D_003EF8E0;
}

void *func_002A8EC0(void *self, s32 i) {
    return D_003F0310[i];
}

void *func_002A8EE0(void) {
    return D_003F0410;
}

void *func_002A8EF0(void) {
    return D_003F0430;
}

void *func_002A8F00(void *self, s32 i) {
    return D_003F03E0[i];
}

/* (self->*D_01990760[i])(a, b) */
s32 func_002A8F20(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990760[i & 0xFF], a, b);
}

/* (self->*D_01990730[i])(a, b) */
s32 func_002A9050(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990730[i & 0xFF], a, b);
}

void *func_002A9660(void) {
    return D_003F0450;
}

void *func_002A9670(void) {
    return D_003F0540;
}

void *func_002A9680(void) {
    return D_003F0580;
}

void *func_002A9690(void) {
    return D_003F06A0;
}

void *func_002A96A0(void) {
    return D_003F0820;
}

void *func_002A96B0(void) {
    return D_003F0860;
}

void *func_002A96C0(void *self, s32 i) {
    return D_003F0D60[i];
}

void *func_002A96E0(void) {
    return D_003F0DD0;
}

void *func_002A96F0(void *self, s32 i) {
    return D_003F0DB0[i];
}

/* (self->*D_01990780[i])(a, b) */
s32 func_002A9710(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990780[i & 0xFF], a, b);
}

void *func_002A9AB0(void) {
    return D_003F0DF0;
}

void *func_002A9AC0(void) {
    return D_003F0E80;
}

void *func_002A9AD0(void) {
    return D_003F0F70;
}

void *func_002A9AE0(void) {
    return D_003F1060;
}

void *func_002A9AF0(void *self, s32 i) {
    return D_003F1740[i];
}

void *func_002A9B10(void) {
    return D_003F17F0;
}

void *func_002A9B20(void *self, s32 i) {
    return D_003F17B0[i];
}

/* (self->*D_019907A0[i])(a, b) */
s32 func_002A9B40(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019907A0[i & 0xFF], a, b);
}

void *func_002AA2B0(void) {
    return D_003F1B60;
}

void *func_002AA2C0(void) {
    return D_003F1BD0;
}

void *func_002AA2D0(void) {
    return D_003F1CD0;
}

void *func_002AA2E0(void) {
    return D_003F1D58;
}

void *func_002AA2F0(void) {
    return D_003F1D70;
}

void *func_002AA300(void) {
    return D_003F1F00;
}

void *func_002AA310(void *self, s32 i) {
    return D_003F2170[i];
}

void *func_002AA330(void *self, s32 i) {
    return D_003F21D0[i];
}

/* (self->*D_019907D0[i])(a, b) */
s32 func_002AA350(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019907D0[i & 0xFF], a, b);
}

void *func_002AA4A0(void) {
    return D_003F2500;
}

void *func_002AA4B0(void) {
    return D_003F2640;
}

void *func_002AA4C0(void) {
    return D_003F2780;
}

void *func_002AA4D0(void) {
    return D_003F28A0;
}

void *func_002AA4E0(void) {
    return D_003F2910;
}

void *func_002AA4F0(void) {
    return D_003F2980;
}

void *func_002AA500(void *self, s32 i) {
    return D_003F3180[i];
}

void *func_002AA520(void) {
    return D_003F3230;
}

void *func_002AA530(void *self, s32 i) {
    return D_003F3200[i];
}

/* (self->*D_01990800[i])(a, b) */
s32 func_002AA550(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990800[i & 0xFF], a, b);
}

/* (self->*D_019907E0[i])(a, b) */
s32 func_002AA5D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019907E0[i & 0xFF], a, b);
}

void *func_002AA980(void) {
    return D_003F3240;
}

void *func_002AA990(void) {
    return D_003F32D0;
}

void *func_002AA9A0(void) {
    return D_003F3330;
}

void *func_002AA9B0(void) {
    return D_003F3400;
}

void *func_002AA9C0(void) {
    return D_003F34A0;
}

void *func_002AA9D0(void) {
    return D_003F34D8;
}

void *func_002AA9E0(void *self, s32 i) {
    return D_003F3C00[i];
}

void *func_002AAA00(void) {
    return D_003F3C80;
}

void *func_002AAA10(void *self, s32 i) {
    return D_003F3C60[i];
}

/* (self->*D_01990818[i])(a, b) */
s32 func_002AAA30(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990818[i & 0xFF], a, b);
}

void *func_002AAB60(void) {
    return D_003F3C90;
}

void *func_002AAB70(void) {
    return D_003F3CF0;
}

void *func_002AAB80(void) {
    return D_003F3DA0;
}

void *func_002AAB90(void) {
    return D_003F3E90;
}

void *func_002AABA0(void) {
    return D_003F3EF0;
}

void *func_002AABB0(void *self, s32 i) {
    return D_003F4350[i];
}

void *func_002AABD0(void) {
    return D_003F43B0;
}

void *func_002AABE0(void *self, s32 i) {
    return D_003F4398[i];
}

/* (self->*D_01990830[i])(a, b) */
s32 func_002AAC00(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990830[i & 0xFF], a, b);
}

void *func_002AB090(void) {
    return D_003F43C0;
}

void *func_002AB0A0(void) {
    return D_003F43E0;
}

void *func_002AB0B0(void) {
    return D_003F4470;
}

void *func_002AB0C0(void *self, s32 i) {
    return D_003F4620[i];
}

/* (self->*D_01990848[i])(a, b) */
s32 func_002AB100(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990848[i & 0xFF], a, b);
}

void *func_002AB250(void) {
    return D_003F4650;
}

void *func_002AB260(void) {
    return D_003F46E0;
}

void *func_002AB270(void) {
    return D_003F4720;
}

void *func_002AB280(void) {
    return D_003F4850;
}

void *func_002AB290(void *self, s32 i) {
    return D_003F53B0[i];
}

void *func_002AB2B0(void *self, s32 i) {
    return D_003F5440[i];
}

/* (self->*D_01990860[i])(a, b) */
s32 func_002AB2D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990860[i & 0xFF], a, b);
}

void *func_002AB890(void) {
    return D_003F5490;
}

void *func_002AB8A0(void) {
    return D_003F54D0;
}

void *func_002AB950(void) {
    return D_003F5540;
}

void *func_002AB960(void) {
    return D_003F5610;
}

void *func_002AB970(void) {
    return D_003F5710;
}

void *func_002AB980(void) {
    return D_003F57E0;
}

void *func_002AB990(void) {
    return D_003F5980;
}

void *func_002AB9A0(void *self, s32 i) {
    return D_003F5E80[i];
}

void *func_002AB9C0(void) {
    return D_003F5EA0;
}

void *func_002ABA50(void) {
    return D_003F5EC0;
}

void *func_002ABA60(void) {
    return D_003F5FC0;
}

void *func_002ABA70(void) {
    return D_003F6120;
}

void *func_002ABA80(void) {
    return D_003F6420;
}

void *func_002ABA90(void *self, s32 i) {
    return D_003F6E70[i];
}

void *func_002ABAB0(void) {
    return D_003F6F68;
}

void *func_002ABAC0(void *self, s32 i) {
    return D_003F6F40[i];
}

/* (self->*D_01990890[i])(a, b) */
s32 func_002ABAE0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990890[i & 0xFF], a, b);
}

void *func_002ABF60(void) {
    return D_003F6F80;
}

void *func_002ABF70(void) {
    return D_003F7060;
}

void *func_002ABF80(void) {
    return D_003F7150;
}

void *func_002ABF90(void) {
    return D_003F72D0;
}

void *func_002ABFA0(void) {
    return D_003F73B0;
}

void *func_002ABFB0(void) {
    return D_003F74E0;
}

void *func_002ABFC0(void *self, s32 i) {
    return D_003F78D0[i];
}

void *func_002ABFE0(void) {
    return D_003F7910;
}

/* (self->*D_019908D0[i])(a, b) */
s32 func_002AC010(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019908D0[i & 0xFF], a, b);
}

void *func_002AC100(void) {
    return D_003F7920;
}

void *func_002AC110(void) {
    return D_003F7A20;
}

void *func_002AC120(void) {
    return D_003F7AC0;
}

void *func_002AC130(void) {
    return D_003F7E00;
}

void *func_002AC140(void *self, s32 i) {
    return D_003F81D0[i];
}

void *func_002AC160(void) {
    return D_003F81F0;
}

void *func_002AC1F0(void) {
    return D_003F8200;
}

void *func_002AC200(void) {
    return D_003F8350;
}

void *func_002AC210(void) {
    return D_003F83F0;
}

void *func_002AC220(void) {
    return D_003F8750;
}

void *func_002AC230(void) {
    return D_003F8900;
}

void *func_002AC240(void) {
    return D_003F89D0;
}

void *func_002AC250(void *self, s32 i) {
    return D_003F90F0[i];
}

void *func_002AC270(void) {
    return D_003F9190;
}

void *func_002AC280(void *self, s32 i) {
    return D_003F9170[i];
}

/* (self->*D_019908F0[i])(a, b) */
s32 func_002AC2A0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019908F0[i & 0xFF], a, b);
}

/* (self->*D_019908E0[i])(a, b) */
s32 func_002AC340(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019908E0[i & 0xFF], a, b);
}

void *func_002AC530(void) {
    return D_003F91A0;
}

void *func_002AC540(void) {
    return D_003F92E0;
}

void *func_002AC550(void) {
    return D_003F93A0;
}

void *func_002AC560(void) {
    return D_003F9530;
}

void *func_002AC570(void) {
    return D_003F95F0;
}

void *func_002AC580(void *self, s32 i) {
    return D_003F9980[i];
}

void *func_002AC5A0(void) {
    return D_003F99C0;
}

void *func_002AC5B0(void *self, s32 i) {
    return D_003F99B0[i];
}

/* (self->*D_01990900[i])(a, b) */
s32 func_002AC5D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990900[i & 0xFF], a, b);
}

void *func_002AC6D0(void) {
    return D_003F99E0;
}

void *func_002AC6E0(void) {
    return D_003F9A30;
}

void *func_002AC6F0(void) {
    return D_003F9AC0;
}

void *func_002AC700(void) {
    return D_003F9B60;
}

void *func_002AC710(void *self, s32 i) {
    return D_003FA030[i];
}

void *func_002AC730(void) {
    return D_003FA080;
}

void *func_002AC740(void *self, s32 i) {
    return D_003FA070[i];
}

/* (self->*D_01990910[i])(a, b) */
s32 func_002AC760(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990910[i & 0xFF], a, b);
}

void *func_002ACBC0(void) {
    return D_003FA090;
}

void *func_002ACBD0(void) {
    return D_003FA110;
}

void *func_002ACBE0(void) {
    return D_003FA1C0;
}

void *func_002ACBF0(void) {
    return D_003FA330;
}

void *func_002ACC00(void *self, s32 i) {
    return D_003FA700[i];
}

void *func_002ACC20(void) {
    return D_003FA2C0;
}

void *func_002ACC30(void) {
    return D_003FA770;
}

void *func_002ACC40(void *self, s32 i) {
    return D_003FA750[i];
}

/* (self->*D_01990930[i])(a, b) */
s32 func_002ACC60(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990930[i & 0xFF], a, b);
}

/* (self->*D_01990920[i])(a, b) */
s32 func_002ACD20(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990920[i & 0xFF], a, b);
}

void *func_002AD080(void) {
    return D_003FAA20;
}

void *func_002AD090(void) {
    return D_003FAAA0;
}

void *func_002AD0A0(void) {
    return D_003FAB30;
}

void *func_002AD0B0(void) {
    return D_003FACB0;
}

void *func_002AD0C0(void) {
    return D_003FAD40;
}

void *func_002AD0E0(void *self, s32 i) {
    return D_003FB320[i];
}

void *func_002AD100(void) {
    return D_003FB390;
}

void *func_002AD110(void *self, s32 i) {
    return D_003FB370[i];
}

/* (self->*D_01990940[i])(a, b) */
s32 func_002AD130(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990940[i & 0xFF], a, b);
}

void *func_002AD220(void) {
    return D_003FB3B0;
}

void *func_002AD230(void) {
    return D_003FB420;
}

void *func_002AD240(void) {
    return D_003FB520;
}

void *func_002AD250(void) {
    return D_003FB720;
}

void *func_002AD260(void) {
    return D_003FB840;
}

void *func_002AD270(void *self, s32 i) {
    return D_003FBFD0[i];
}

void *func_002AD290(void) {
    return D_003FC040;
}

void *func_002AD2A0(void *self, s32 i) {
    return D_003FC020[i];
}

/* (self->*D_01990950[i])(a, b) */
s32 func_002AD2C0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990950[i & 0xFF], a, b);
}

void *func_002AD480(void) {
    return D_003FC060;
}

void *func_002AD490(void) {
    return D_003FC0A0;
}

void *func_002AD4A0(void) {
    return D_003FC110;
}

void *func_002AD4B0(void) {
    return D_003FC220;
}

void *func_002AD4C0(void) {
    return D_003FC290;
}

void *func_002AD4D0(void *self, s32 i) {
    return D_003FC630[i];
}

void *func_002AD4F0(void) {
    return D_003FC690;
}

void *func_002AD500(void *self, s32 i) {
    return D_003FC678[i];
}

/* (self->*D_01990960[i])(a, b) */
s32 func_002AD520(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990960[i & 0xFF], a, b);
}

void *func_002AD6F0(void) {
    return D_003FC6A0;
}

void *func_002AD700(void) {
    return D_003FC6F0;
}

void *func_002AD710(void) {
    return D_003FC770;
}

void *func_002AD720(void) {
    return D_003FC8F0;
}

void *func_002AD730(void *self, s32 i) {
    return D_003FCAD0[i];
}

void *func_002AD750(void) {
    return D_003FCB18;
}

void *func_002AD760(void *self, s32 i) {
    return D_003FCB00[i];
}

/* (self->*D_01990978[i])(a, b) */
s32 func_002AD780(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990978[i & 0xFF], a, b);
}

void *func_002ADA50(void) {
    return D_003FCB30;
}

void *func_002ADA60(void) {
    return D_003FCBC0;
}

void *func_002ADA70(void) {
    return D_003FCC40;
}

void *func_002ADA80(void) {
    return D_003FCD70;
}

/* (self->*D_01990990[i])(a, b) */
s32 func_002ADAE0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990990[i & 0xFF], a, b);
}

/* (self->*D_019909C8[i])(a, b) */
s32 func_002ADE00(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019909C8[i & 0xFF], a, b);
}

/* (self->*D_019909B0[i])(a, b) */
s32 func_002ADEB0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019909B0[i & 0xFF], a, b);
}

/* (self->*D_019909D8[i])(a, b) */
s32 func_002AE180(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019909D8[i & 0xFF], a, b);
}

/* (self->*D_01990A40[i])(a, b) */
s32 func_002AE4B0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990A40[i & 0xFF], a, b);
}

/* (self->*D_019909F0[i])(a, b) */
s32 func_002AE760(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019909F0[i & 0xFF], a, b);
}

/* (self->*D_01990AC8[i])(a, b) */
s32 func_002AF0C0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990AC8[i & 0xFF], a, b);
}

/* (self->*D_01990A70[i])(a, b) */
s32 func_002AF1A0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990A70[i & 0xFF], a, b);
}

/* (self->*D_01990AF0[i])(a, b) */
s32 func_002AF9D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990AF0[i & 0xFF], a, b);
}

/* (self->*D_01990AD8[i])(a, b) */
s32 func_002AFB20(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990AD8[i & 0xFF], a, b);
}

/* (self->*D_01990B10[i])(a, b) */
s32 func_002AFD30(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990B10[i & 0xFF], a, b);
}

extern PTMF D_01990BD0[];

/* (self->*D_01990BD0[i])(a, b) */
s32 func_002B11A0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990BD0[i & 0xFF], a, b);
}

/* (self->*D_01990B48[i])(a, b) */
s32 func_002B0200(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990B48[i & 0xFF], a, b);
}

/* (self->*D_01990B38[i])(a, b) */
s32 func_002B02A0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990B38[i & 0xFF], a, b);
}

/* (self->*D_01990B70[i])(a, b) */
s32 func_002B04B0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990B70[i & 0xFF], a, b);
}

/* (self->*D_01990B58[i])(a, b) */
s32 func_002B0C60(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990B58[i & 0xFF], a, b);
}

/* (self->*D_01990B98[i])(a, b) */
s32 func_002B0DF0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990B98[i & 0xFF], a, b);
}

/* (self->*D_01990BB0[i])(a, b) */
s32 func_002B0F30(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990BB0[i & 0xFF], a, b);
}

/* (self->*D_01990C00[i])(a, b) */
s32 func_002B16D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990C00[i & 0xFF], a, b);
}

/* (self->*D_01990C38[i])(a, b) */
s32 func_002B1880(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990C38[i & 0xFF], a, b);
}

/* (self->*D_01990C10[i])(a, b) */
s32 func_002B19D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990C10[i & 0xFF], a, b);
}

/* (self->*D_01990C48[i])(a, b) */
s32 func_002B2420(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990C48[i & 0xFF], a, b);
}

/* (self->*D_01990C58[i])(a, b) */
s32 func_002B26C0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990C58[i & 0xFF], a, b);
}

/* (self->*D_01990C68[i])(a, b) */
s32 func_002B2920(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990C68[i & 0xFF], a, b);
}

/* (self->*D_01990C80[i])(a, b) */
s32 func_002B2A50(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990C80[i & 0xFF], a, b);
}

/* (self->*D_01990CC8[i])(a, b) */
s32 func_002B3100(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990CC8[i & 0xFF], a, b);
}

/* (self->*D_01990CB0[i])(a, b) */
s32 func_002B3170(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990CB0[i & 0xFF], a, b);
}

/* (self->*D_01990CE0[i])(a, b) */
s32 func_002B3630(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990CE0[i & 0xFF], a, b);
}

/* (self->*D_01990D10[i])(a, b) */
s32 func_002B3A70(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990D10[i & 0xFF], a, b);
}

/* (self->*D_01990D70[i])(a, b) */
s32 func_002B4000(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990D70[i & 0xFF], a, b);
}

/* (self->*D_01990D40[i])(a, b) */
s32 func_002B4250(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990D40[i & 0xFF], a, b);
}

/* (self->*D_01990D80[i])(a, b) */
s32 func_002B4790(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990D80[i & 0xFF], a, b);
}

/* (self->*D_01990D90[i])(a, b) */
s32 func_002B4940(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990D90[i & 0xFF], a, b);
}

/* (self->*D_01990DB8[i])(a, b) */
s32 func_002B4B40(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990DB8[i & 0xFF], a, b);
}

/* (self->*D_01990DA8[i])(a, b) */
s32 func_002B4BB0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990DA8[i & 0xFF], a, b);
}

/* (self->*D_01990DC8[i])(a, b) */
s32 func_002B4DB0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990DC8[i & 0xFF], a, b);
}

/* (self->*D_01990DD8[i])(a, b) */
s32 func_002B51E0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990DD8[i & 0xFF], a, b);
}

/* (self->*D_01990DE8[i])(a, b) */
s32 func_002B5380(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990DE8[i & 0xFF], a, b);
}

/* (self->*D_01990E08[i])(a, b) */
s32 func_002B59B0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990E08[i & 0xFF], a, b);
}

/* (self->*D_01990DF8[i])(a, b) */
s32 func_002B5C00(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990DF8[i & 0xFF], a, b);
}

/* (self->*D_01990E20[i])(a, b) */
s32 func_002B5F70(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990E20[i & 0xFF], a, b);
}

/* (self->*D_01990E38[i])(a, b) */
s32 func_002CCA50(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990E38[i & 0xFF], a, b);
}

/* (self->*D_01990E48[i])(a, b) */
s32 func_002CCB90(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990E48[i & 0xFF], a, b);
}

/* (self->*D_01990E88[i])(a, b) */
s32 func_002E5850(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990E88[i & 0xFF], a, b);
}

/* (self->*D_01990E78[i])(a, b) */
s32 func_002E5920(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990E78[i & 0xFF], a, b);
}

/* (self->*D_01990E98[i])(a, b) */
s32 func_002E5C30(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990E98[i & 0xFF], a, b);
}

/* (self->*D_01990EA8[i])(a, b) */
s32 func_002E60A0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990EA8[i & 0xFF], a, b);
}

/* (self->*D_01990EB8[i])(a, b) */
s32 func_002E6510(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990EB8[i & 0xFF], a, b);
}

/* (self->*D_01990EC8[i])(a, b) */
s32 func_002E6980(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990EC8[i & 0xFF], a, b);
}

/* (self->*D_01990ED8[i])(a, b) */
s32 func_002E6DF0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990ED8[i & 0xFF], a, b);
}

/* (self->*D_01990F00[i])(a, b) */
s32 func_002E6FF0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F00[i & 0xFF], a, b);
}

/* (self->*D_01990EE8[i])(a, b) */
s32 func_002E7190(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990EE8[i & 0xFF], a, b);
}

/* (self->*D_01990F18[i])(a, b) */
s32 func_002E7320(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F18[i & 0xFF], a, b);
}

/* (self->*D_01990F30[i])(a, b) */
s32 func_002E7530(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F30[i & 0xFF], a, b);
}

/* (self->*D_01990F58[i])(a, b) */
s32 func_002E7780(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F58[i & 0xFF], a, b);
}

/* (self->*D_01990F48[i])(a, b) */
s32 func_002E7850(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F48[i & 0xFF], a, b);
}

/* (self->*D_01990F78[i])(a, b) */
s32 func_002E79D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F78[i & 0xFF], a, b);
}

/* (self->*D_01990F68[i])(a, b) */
s32 func_002E7A20(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F68[i & 0xFF], a, b);
}

void *func_002ECC90(void) {
    return D_00419DD0;
}

void *func_002ECCA0(void) {
    return D_00419E10;
}

void *func_002F6DC0(void) {
    return D_0045E4C0;
}

void *func_002F6DD0(void) {
    return D_0045E4E0;
}

void *func_002F8930(void) {
    return D_0041A5F0;
}

void *func_002FA1C0(void) {
    return D_0041B5F0;
}

void *func_002FA320(void) {
    return D_0041BE40;
}

void *func_002FCBA0(void) {
    return D_0041C390;
}

void *func_002FCBC0(void) {
    return D_0041C3E0;
}

void *func_002FCBD0(void) {
    return D_0041C420;
}

void *func_002FCBE0(void *self, s32 i) {
    return D_0041CA40[i];
}

/* (self->*D_01990FD8[i])(a, b) */
s32 func_002FCC00(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990FD8[i & 0xFF], a, b);
}

void *func_002FEF20(void) {
    return D_0041D140;
}

void *func_002FEF30(void) {
    return D_0041D1C0;
}

void *func_002FEFE0(void) {
    return D_0041D2E0;
}

void *func_002FEFF0(void) {
    return D_0041D330;
}

void *func_002FF000(void) {
    return D_0041D3C0;
}

void *func_002FF020(void) {
    return D_0041D4E0;
}

void *func_002FF030(void *self, s32 i) {
    return D_0041D7A0[i];
}

void *func_002FF050(void *self, s32 i) {
    return D_0041D808[i];
}

/* (self->*D_01990FF0[i])(a, b) */
s32 func_002FF070(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990FF0[i & 0xFF], a, b);
}

void *func_00300210(void) {
    return D_0041E110;
}

void *func_00300220(void) {
    return D_0041E260;
}

void *func_00300230(void) {
    return D_0041E360;
}

void *func_00300240(void) {
    return D_0041E700;
}

void *func_00300260(void *self, s32 i) {
    return D_0041F4E0[i];
}

void *func_00300280(void) {
    return D_0041F5C0;
}

/* (self->*D_01991070[i])(a, b) */
s32 func_003002B0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991070[i & 0xFF], a, b);
}

/* (self->*D_01991030[i])(a, b) */
s32 func_003004F0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991030[i & 0xFF], a, b);
}

/* (self->*D_01991088[i])(a, b) */
s32 func_00305F80(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991088[i & 0xFF], a, b);
}

/* (self->*D_01991098[i])(a, b) */
s32 func_00308A90(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991098[i & 0xFF], a, b);
}

/* (self->*D_019910B0[i])(a, b) */
s32 func_00308BF0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019910B0[i & 0xFF], a, b);
}

/* (self->*D_01991188[i])(a, b) */
s32 func_0030F170(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991188[i & 0xFF], a, b);
}

/* (self->*D_01991140[i])(a, b) */
s32 func_0030F1C0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991140[i & 0xFF], a, b);
}

/* (self->*D_019911A0[i])(a, b) */
s32 func_0030FA30(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019911A0[i & 0xFF], a, b);
}

/* (self->*D_019911C0[i])(a, b) */
s32 func_00310250(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019911C0[i & 0xFF], a, b);
}

/* (self->*D_01991550[i])(a, b) */
s32 func_00311490(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991550[i & 0xFF], a, b);
}

/* (self->*D_01991570[i])(a, b) */
s32 func_0031E290(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991570[i & 0xFF], a, b);
}

/* (self->*D_019915A0[i])(a, b) */
s32 func_00320F40(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019915A0[i & 0xFF], a, b);
}

/* (self->*D_019915E8[i])(a, b) */
s32 func_0032C770(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019915E8[i & 0xFF], a, b);
}

/* (self->*D_01991650[i])(a, b) */
s32 func_0032DCE0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991650[i & 0xFF], a, b);
}

/* (self->*D_01991610[i])(a, b) */
s32 func_0032DD40(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991610[i & 0xFF], a, b);
}

/* (self->*D_019916C0[i])(a, b) */
s32 func_00339DF0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019916C0[i & 0xFF], a, b);
}

/* (self->*D_01991660[i])(a, b) */
s32 func_00339ED0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991660[i & 0xFF], a, b);
}

/* (self->*D_019916D8[i])(a, b) */
s32 func_0033F280(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019916D8[i & 0xFF], a, b);
}

/* (self->*D_019916E8[i])(a, b) */
s32 func_0033F430(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019916E8[i & 0xFF], a, b);
}

/* (self->*D_019916F8[i])(a, b) */
s32 func_0033F5C0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019916F8[i & 0xFF], a, b);
}

/* (self->*D_01991708[i])(a, b) */
s32 func_0033F780(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991708[i & 0xFF], a, b);
}

/* (self->*D_01991720[i])(a, b) */
s32 func_0033FA30(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991720[i & 0xFF], a, b);
}

/* (self->*D_01991760[i])(a, b) */
s32 func_00340170(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991760[i & 0xFF], a, b);
}

/* (self->*D_01991790[i])(a, b) */
s32 func_00340AA0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991790[i & 0xFF], a, b);
}

/* (self->*D_019917E0[i])(a, b) */
s32 func_00340E60(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019917E0[i & 0xFF], a, b);
}

/* (self->*D_019917B0[i])(a, b) */
s32 func_00340F20(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019917B0[i & 0xFF], a, b);
}

/* (self->*D_019917F0[i])(a, b) */
s32 func_00341660(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019917F0[i & 0xFF], a, b);
}

/* (self->*D_01991820[i])(a, b) */
s32 func_00341C00(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991820[i & 0xFF], a, b);
}

/* (self->*D_019918C8[i])(a, b) */
s32 func_00343800(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019918C8[i & 0xFF], a, b);
}

/* (self->*D_019918D8[i])(a, b) */
s32 func_003439D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019918D8[i & 0xFF], a, b);
}

/* (self->*D_019918F0[i])(a, b) */
s32 func_00344150(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019918F0[i & 0xFF], a, b);
}

/* (self->*D_01991910[i])(a, b) */
s32 func_00344A70(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991910[i & 0xFF], a, b);
}

/* (self->*D_01991930[i])(a, b) */
s32 func_00344C90(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991930[i & 0xFF], a, b);
}

/* (self->*D_01991958[i])(a, b) */
s32 func_00344F80(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991958[i & 0xFF], a, b);
}

/* (self->*D_01991948[i])(a, b) */
s32 func_00344FC0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991948[i & 0xFF], a, b);
}

/* (self->*D_01991968[i])(a, b) */
s32 func_0034A220(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991968[i & 0xFF], a, b);
}

/* (self->*D_01991978[i])(a, b) */
s32 func_0034A460(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991978[i & 0xFF], a, b);
}

/* (self->*D_019919A8[i])(a, b) */
s32 func_0034A5C0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019919A8[i & 0xFF], a, b);
}

/* (self->*D_01991990[i])(a, b) */
s32 func_0034A620(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991990[i & 0xFF], a, b);
}

/* (self->*D_019919E8[i])(a, b) */
s32 func_0034AAB0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019919E8[i & 0xFF], a, b);
}

/* (self->*D_019919C0[i])(a, b) */
s32 func_0034AB10(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019919C0[i & 0xFF], a, b);
}

/* (self->*D_019919F8[i])(a, b) */
s32 func_0034B1E0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019919F8[i & 0xFF], a, b);
}

/* (self->*D_01991A08[i])(a, b) */
s32 func_0034B6B0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A08[i & 0xFF], a, b);
}

/* (self->*D_01991A18[i])(a, b) */
s32 func_00350F30(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A18[i & 0xFF], a, b);
}

/* (self->*D_01991A28[i])(a, b) */
s32 func_00352BD0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A28[i & 0xFF], a, b);
}

/* (self->*D_01991A38[i])(a, b) */
s32 func_00352D60(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A38[i & 0xFF], a, b);
}

/* (self->*D_01991A48[i])(a, b) */
s32 func_00352F20(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A48[i & 0xFF], a, b);
}

void *func_0035AFE0(void) {
    return D_00463320;
}

void *func_0035AFF0(void) {
    return D_00463340;
}

void *func_0035D200(void) {
    return D_00444170;
}

void *func_0035D210(void) {
    return D_004441C0;
}

void *func_0035D220(void) {
    return D_00444200;
}

void *func_0035D230(void) {
    return D_00444270;
}

void *func_0035D250(void *self, s32 i) {
    return D_00444360[i];
}

/* (self->*D_01991A58[i])(a, b) */
s32 func_0035D270(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A58[i & 0xFF], a, b);
}

void *func_0035D390(void) {
    return D_00444390;
}

void *func_0035D3A0(void) {
    return D_004443C0;
}

void *func_0035D3B0(void) {
    return D_00444400;
}

void *func_0035D3C0(void) {
    return D_00444460;
}

void *func_0035D3E0(void *self, s32 i) {
    return D_004444C0[i];
}

/* (self->*D_01991A68[i])(a, b) */
s32 func_0035D400(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A68[i & 0xFF], a, b);
}

void *func_0035D520(void) {
    return D_004444E0;
}

void *func_0035D530(void) {
    return D_00444530;
}

void *func_0035D540(void) {
    return D_00444570;
}

void *func_0035D550(void) {
    return D_004445D0;
}

void *func_0035D570(void *self, s32 i) {
    return D_004446B0[i];
}

/* (self->*D_01991A78[i])(a, b) */
s32 func_0035D590(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A78[i & 0xFF], a, b);
}

void *func_0035D6B0(void) {
    return D_004446E0;
}

void *func_0035D6C0(void) {
    return D_00444710;
}

void *func_0035D6D0(void) {
    return D_00444750;
}

void *func_0035D6E0(void) {
    return D_004447C0;
}

void *func_0035D700(void *self, s32 i) {
    return D_00444830[i];
}

/* (self->*D_01991A88[i])(a, b) */
s32 func_0035D720(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A88[i & 0xFF], a, b);
}

void *func_00363600(void) {
    return D_00444B10;
}

void *func_0036A360(void) {
    return D_00445BC0;
}

void *func_0036A370(void) {
    return D_00445C20;
}

void *func_0036A380(void) {
    return D_00445C70;
}

void *func_0036A390(void) {
    return D_00445C90;
}

void *func_0036A3A0(void) {
    return D_00445D70;
}

void *func_0036A3B0(void *self, s32 i) {
    return D_00446960[i];
}

/* (self->*D_01991AB0[i])(a, b) */
s32 func_0036A3D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991AB0[i & 0xFF], a, b);
}

void *func_0036DA90(void) {
    return D_004469C0;
}

void *func_0036DAA0(void) {
    return D_00446A00;
}

void *func_0036DAB0(void) {
    return D_00446A80;
}

void *func_0036DAD0(void) {
    return D_00446B00;
}

void *func_0036DB00(void) {
    return D_00446BE0;
}

/* (self->*D_01991AE8[i])(a, b) */
s32 func_0036DB10(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991AE8[i & 0xFF], a, b);
}

/* (self->*D_01991AC0[i])(a, b) */
s32 func_0036DB60(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991AC0[i & 0xFF], a, b);
}

void *func_0036DF90(void) {
    return D_00446C00;
}

void *func_0036DFA0(void) {
    return D_00446C10;
}

void *func_0036DFB0(void) {
    return D_00446C90;
}

void *func_0036DFD0(void) {
    return D_00446CF0;
}

void *func_0036E000(void) {
    return D_00446D40;
}

/* (self->*D_01991B18[i])(a, b) */
s32 func_0036E010(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B18[i & 0xFF], a, b);
}

/* (self->*D_01991B00[i])(a, b) */
s32 func_0036E060(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B00[i & 0xFF], a, b);
}

void *func_0036E340(void) {
    return D_00446D60;
}

void *func_0036E350(void) {
    return D_00446E60;
}

void *func_0036E360(void) {
    return D_00446EE0;
}

void *func_0036E370(void) {
    return D_00446F20;
}

/* (self->*D_01991B38[i])(a, b) */
s32 func_0036E380(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B38[i & 0xFF], a, b);
}

/* (self->*D_01991B28[i])(a, b) */
s32 func_0036E3D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B28[i & 0xFF], a, b);
}

void *func_0036E5C0(void) {
    return D_00446F30;
}

void *func_0036E5D0(void) {
    return D_00447030;
}

void *func_0036E5F0(void) {
    return D_004470D0;
}

void *func_0036E620(void) {
    return D_00447110;
}

/* (self->*D_01991B58[i])(a, b) */
s32 func_0036E630(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B58[i & 0xFF], a, b);
}

/* (self->*D_01991B48[i])(a, b) */
s32 func_0036E680(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B48[i & 0xFF], a, b);
}

void *func_0036E870(void) {
    return D_00447130;
}

void *func_0036E880(void) {
    return D_00447230;
}

void *func_0036E890(void) {
    return D_004472B0;
}

void *func_0036E8A0(void) {
    return D_004472F0;
}

/* (self->*D_01991B78[i])(a, b) */
s32 func_0036E8B0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B78[i & 0xFF], a, b);
}

/* (self->*D_01991B68[i])(a, b) */
s32 func_0036E900(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B68[i & 0xFF], a, b);
}

void *func_0036EAE0(void) {
    return D_00447300;
}

void *func_0036EAF0(void) {
    return D_00447330;
}

void *func_0036EB00(void) {
    return D_00447460;
}

void *func_0036EB10(void) {
    return D_00447560;
}

void *func_0036EB20(void) {
    return D_00447550;
}

void *func_0036EB30(void *self, s32 i) {
    return D_00447608[i];
}

void *func_0036EB50(void) {
    return D_00447660;
}

/* (self->*D_01991BB8[i])(a, b) */
s32 func_0036EB60(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991BB8[i & 0xFF], a, b);
}

/* (self->*D_01991B90[i])(a, b) */
s32 func_0036EBB0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B90[i & 0xFF], a, b);
}

void *func_0036EEE0(void) {
    return D_00447680;
}

void *func_0036EEF0(void) {
    return D_00447790;
}

void *func_0036EF10(void) {
    return D_00447890;
}

void *func_0036EF20(void *self, s32 i) {
    return D_00447A10[i];
}

void *func_0036EF40(void) {
    return D_004478B0;
}

void *func_0036EF50(void) {
    return D_00447A60;
}

void *func_0036EF60(void *self, s32 i) {
    return D_00447A50[i];
}

/* (self->*D_01991BE8[i])(a, b) */
s32 func_0036EF80(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991BE8[i & 0xFF], a, b);
}

/* (self->*D_01991BD0[i])(a, b) */
s32 func_0036EFD0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991BD0[i & 0xFF], a, b);
}

void *func_0036F500(void) {
    return D_00447A70;
}

void *func_0036F510(void) {
    return D_00447A90;
}

void *func_0036F520(void) {
    return D_00447AD0;
}

void *func_0036F540(void) {
    return D_00447B00;
}

void *func_0036F580(void) {
    return D_00447BC0;
}

/* (self->*D_01991C20[i])(a, b) */
s32 func_0036F590(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991C20[i & 0xFF], a, b);
}

/* (self->*D_01991C00[i])(a, b) */
s32 func_0036F880(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991C00[i & 0xFF], a, b);
}

void *func_0036F990(void) {
    return D_00447BD0;
}

void *func_0036F9A0(void) {
    return D_00447BF0;
}

void *func_0036F9B0(void) {
    return D_00447CF0;
}

void *func_0036F9D0(void) {
    return D_00447DA0;
}

void *func_0036FA10(void) {
    return D_00447E60;
}

/* (self->*D_01991C70[i])(a, b) */
s32 func_0036FA20(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991C70[i & 0xFF], a, b);
}

/* (self->*D_01991C50[i])(a, b) */
s32 func_0036FC40(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991C50[i & 0xFF], a, b);
}

void *func_0036FD50(void) {
    return D_00447E80;
}

void *func_0036FD60(void) {
    return D_00447E90;
}

void *func_0036FD70(void) {
    return D_00447F40;
}

void *func_0036FD80(void) {
    return D_00447F60;
}

void *func_0036FDB0(void) {
    return D_00447FC8;
}

/* (self->*D_01991CB8[i])(a, b) */
s32 func_0036FDC0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991CB8[i & 0xFF], a, b);
}

/* (self->*D_01991C90[i])(a, b) */
s32 func_0036FE10(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991C90[i & 0xFF], a, b);
}

void *func_003788D0(void) {
    return D_004480B0;
}

void *func_003788E0(void) {
    return D_004480F0;
}

void *func_003788F0(void) {
    return D_00448200;
}

void *func_00378910(void) {
    return D_00448320;
}

void *func_00378920(void *self, s32 i) {
    return D_004484E0[i];
}

void *func_00378940(void) {
    return D_00448340;
}

void *func_00378950(void) {
    return D_00448570;
}

/* (self->*D_01991D18[i])(a, b) */
s32 func_00378980(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D18[i & 0xFF], a, b);
}

/* (self->*D_01991CD0[i])(a, b) */
s32 func_003789D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991CD0[i & 0xFF], a, b);
}

void *func_00379040(void) {
    return D_00448590;
}

void *func_00379050(void) {
    return D_004485E0;
}

void *func_00379060(void) {
    return D_00448660;
}

void *func_00379080(void) {
    return D_00448740;
}

void *func_003790B0(void) {
    return D_004488A0;
}

/* (self->*D_01991D38[i])(a, b) */
s32 func_003790C0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D38[i & 0xFF], a, b);
}

/* (self->*D_01991D28[i])(a, b) */
s32 func_00379110(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D28[i & 0xFF], a, b);
}

void *func_003792F0(void) {
    return D_004488B0;
}

void *func_00379300(void) {
    return D_00448990;
}

void *func_00379310(void) {
    return D_00448A10;
}

void *func_00379330(void) {
    return D_00448CD0;
}

void *func_00379360(void) {
    return D_00448D10;
}

/* (self->*D_01991D58[i])(a, b) */
s32 func_00379370(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D58[i & 0xFF], a, b);
}

/* (self->*D_01991D48[i])(a, b) */
s32 func_003793C0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D48[i & 0xFF], a, b);
}

void *func_003795A0(void) {
    return D_00448D20;
}

void *func_003795B0(void) {
    return D_00448DA0;
}

void *func_003795C0(void) {
    return D_00448E30;
}

void *func_003795E0(void *self, s32 i) {
    return D_00449210[i];
}

void *func_00379600(void) {
    return D_00448F30;
}

void *func_00379610(void) {
    return D_00449010;
}

void *func_00379620(void) {
    return D_00449240;
}

/* (self->*D_01991D78[i])(a, b) */
s32 func_00379650(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D78[i & 0xFF], a, b);
}

/* (self->*D_01991D68[i])(a, b) */
s32 func_003796A0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D68[i & 0xFF], a, b);
}

void *func_00379880(void) {
    return D_00449250;
}

void *func_00379890(void) {
    return D_004492C0;
}

void *func_003798A0(void) {
    return D_00449380;
}

void *func_003798C0(void) {
    return D_004494F0;
}

void *func_003798F0(void) {
    return D_00449558;
}

void *func_00379900(void *self, s32 i) {
    return D_00449540[i];
}

/* (self->*D_01991DA8[i])(a, b) */
s32 func_00379920(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991DA8[i & 0xFF], a, b);
}

/* (self->*D_01991D90[i])(a, b) */
s32 func_00379970(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D90[i & 0xFF], a, b);
}

void *func_00379C80(void) {
    return D_00449570;
}

void *func_00379C90(void) {
    return D_00449590;
}

void *func_00379CA0(void) {
    return D_004495E0;
}

void *func_00379CC0(void) {
    return D_00449640;
}

void *func_00379CD0(void *self, s32 i) {
    return D_00449890[i];
}

void *func_00379CF0(void) {
    return D_00449660;
}

void *func_00379D00(void) {
    return D_004498E0;
}

/* (self->*D_01991DD8[i])(a, b) */
s32 func_00379D30(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991DD8[i & 0xFF], a, b);
}

/* (self->*D_01991DC0[i])(a, b) */
s32 func_00379D80(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991DC0[i & 0xFF], a, b);
}

void *func_0037A330(void) {
    return D_004498F0;
}

void *func_0037A340(void) {
    return D_00449910;
}

void *func_0037A350(void) {
    return D_00449990;
}

void *func_0037A360(void) {
    return D_004499E0;
}

void *func_0037A3A0(void) {
    return D_00449AC0;
}

/* (self->*D_01991E08[i])(a, b) */
s32 func_0037A3D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991E08[i & 0xFF], a, b);
}

/* (self->*D_01991DF0[i])(a, b) */
s32 func_0037A420(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991DF0[i & 0xFF], a, b);
}

void *func_0037A650(void) {
    return D_00449AD0;
}

void *func_0037A660(void) {
    return D_00449B20;
}

void *func_0037A670(void) {
    return D_00449BB0;
}

void *func_0037A690(void) {
    return D_00449CE0;
}

void *func_0037A6A0(void *self, s32 i) {
    return D_00449E90[i];
}

void *func_0037A6C0(void) {
    return D_00449D00;
}

void *func_0037A6D0(void) {
    return D_00449EC0;
}

/* (self->*D_01991E28[i])(a, b) */
s32 func_0037A700(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991E28[i & 0xFF], a, b);
}

/* (self->*D_01991E18[i])(a, b) */
s32 func_0037A750(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991E18[i & 0xFF], a, b);
}

void *func_0037A940(void) {
    return D_00449EE0;
}

void *func_0037A950(void) {
    return D_00449F60;
}

void *func_0037A970(void) {
    return D_00449FB0;
}

void *func_0037A9A0(void) {
    return D_0044A2A0;
}

/* (self->*D_01991E48[i])(a, b) */
s32 func_0037A9B0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991E48[i & 0xFF], a, b);
}

/* (self->*D_01991E38[i])(a, b) */
s32 func_0037AA00(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991E38[i & 0xFF], a, b);
}

void *func_0037ABE0(void) {
    return D_0044A2C0;
}

void *func_0037ABF0(void) {
    return D_0044A380;
}

void *func_0037AC00(void) {
    return D_0044A3C0;
}

void *func_0037AC20(void) {
    return D_0044A470;
}

void *func_0037AC30(void *self, s32 i) {
    return D_0044A928[i];
}

void *func_0037AC50(void) {
    return D_0044A990;
}

/* (self->*D_01991E90[i])(a, b) */
s32 func_0037AC60(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991E90[i & 0xFF], a, b);
}

/* (self->*D_01991E60[i])(a, b) */
s32 func_0037ACB0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991E60[i & 0xFF], a, b);
}

void *func_0037E3F0(void) {
    return D_00463A50;
}


extern u8 D_0047ACB4[];

void *func_002FEF10(void) {
    return D_0047ACB4;
}
