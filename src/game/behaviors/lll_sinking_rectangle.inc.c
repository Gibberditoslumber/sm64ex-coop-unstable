// lll_sinking_rectangle.c.inc

void sinking_rectangular_plat_actions(f32 a0, s32 a1) {
    switch (o->oAction) {
        case 0:
            o->oAction++;
            break;
        case 1:
            o->oPosY -= sins(o->oLllWoodPieceOscillationTimer) * a0;
            o->oLllWoodPieceOscillationTimer += a1;
            break;
        case 2:
            break;
            break; // Two breaks needed?
    }
}

void bhv_lll_sinking_rectangular_platform_loop(void) {
    if (!network_sync_object_initialized(o)) {
        network_init_object(o, 1000.0f);
        network_init_object_field(o, &o->oLllWoodPieceOscillationTimer);
        network_init_object_field(o, &o->oFaceAnglePitch);
    }
    f32 sp1C = 0.4f;
    s32 sp18 = 0x100;
    if (o->oMoveAngleYaw != 0)
        sinking_rectangular_plat_actions(sp1C, sp18);
    else {
        o->oFaceAnglePitch = sins(o->oLllWoodPieceOscillationTimer) * 512.0f;
        o->oLllWoodPieceOscillationTimer += 0x100;
    }
}

void bhv_lll_sinking_square_platforms_loop(void) {
    if (!network_sync_object_initialized(o)) {
        network_init_object(o, 1000.0f);
        network_init_object_field(o, &o->oLllWoodPieceOscillationTimer);
    }
    f32 sp1C = 0.5f;
    s32 sp18 = 0x100;
    sinking_rectangular_plat_actions(sp1C, sp18);
}
