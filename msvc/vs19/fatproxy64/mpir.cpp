#include <windows.h>
#include <shlwapi.h>
#include <intrin.h>

HINSTANCE hLThis = 0;
FARPROC p[600];
HINSTANCE hL = 0;
char modelstr[100];

void GetCPU() {
    int cpuinfo[4];

    __cpuid(cpuinfo, 0);

    char vendor_string[13];

    vendor_string[0] = cpuinfo[1] & 0xff;
    vendor_string[1] = cpuinfo[1] >> 8 & 0xff;
    vendor_string[2] = cpuinfo[1] >> 16 & 0xff;
    vendor_string[3] = cpuinfo[1] >> 24 & 0xff;

    vendor_string[4] = cpuinfo[3] & 0xff;
    vendor_string[5] = cpuinfo[3] >> 8 & 0xff;
    vendor_string[6] = cpuinfo[3] >> 16 & 0xff;
    vendor_string[7] = cpuinfo[3] >> 24 & 0xff;

    vendor_string[8] = cpuinfo[2] & 0xff;
    vendor_string[9] = cpuinfo[2] >> 8 & 0xff;
    vendor_string[10] = cpuinfo[2] >> 16 & 0xff;
    vendor_string[11] = cpuinfo[2] >> 24 & 0xff;

    vendor_string[12] = 0;

    __cpuid(cpuinfo, 1);

    int family, model, stepping;

    family = ((cpuinfo[0] >> 8) & 15) + ((cpuinfo[0] >> 20) & 0xff);
    model = ((cpuinfo[0] >> 4) & 15) + ((cpuinfo[0] >> 12) & 0xf0);
    stepping = cpuinfo[0] & 15;

    const int AVX2 = 1 << 5;

    strcpy_s(modelstr, sizeof(modelstr), "gc");

    if (strcmp(vendor_string, "GenuineIntel") == 0) {
        switch (family) {
        case 6:
            switch (model) {
            case 60:
            case 63:
            case 69:
            case 70:
                strcpy_s(modelstr, sizeof(modelstr), "haswell");
                //__cpuid(cpuinfo, 7);
                //if ((cpuinfo[1] & AVX2) == AVX2)
                //	strcat_s(modelstr, sizeof(modelstr), "_avx");
                break;
            case 61:
            case 71:
            case 79:
                strcpy_s(modelstr, sizeof(modelstr), "broadwell");
                __cpuid(cpuinfo, 7);
                if ((cpuinfo[1] & AVX2) == AVX2)
                    strcat_s(modelstr, sizeof(modelstr), "_avx");
                break;
            case 78:
            case 85:
            case 94:
            case 102:
            case 106:
            case 108:
            case 125:
            case 126:
            case 140:
            case 142:
            case 158:
                strcpy_s(modelstr, sizeof(modelstr), "skylake");
                __cpuid(cpuinfo, 7);
                if ((cpuinfo[1] & AVX2) == AVX2)
                    strcat_s(modelstr, sizeof(modelstr), "_avx");
                break;
            }
        }
    }
    else if (strcmp(vendor_string, "AuthenticAMD") == 0) {
        switch (family) {
        case 21:
            switch (model) {
            case 1:
                strcpy_s(modelstr, sizeof(modelstr), "bulldozer");
                break;
            case 2:
            case 3:
            case 16:
            case 18:
            case 19:
                strcpy_s(modelstr, sizeof(modelstr), "piledriver");
                break;
            }
            break;
        }
    }
}

BOOL WINAPI DllMain(HINSTANCE hInst,DWORD reason,LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        hLThis = hInst;
        char RealDLL[MAX_PATH + 1];
        GetModuleFileName(hInst, RealDLL, MAX_PATH);
        PathRemoveFileSpec(RealDLL);
        GetCPU();
        strcat_s(RealDLL, MAX_PATH, "\\mpir_");
        strcat_s(RealDLL, MAX_PATH, modelstr);
        strcat_s(RealDLL, MAX_PATH, ".dll");
        hL = LoadLibrary(RealDLL);
        if(!hL) return false;
    }

//	p[0] = GetProcAddress(hL, "??5@YAAEAV?$basic_istream@DU?$char_traits@D@std@@@std@@AEAV01@PEAU__mpf_struct@@@Z");
//	p[1] = GetProcAddress(hL, "??5@YAAEAV?$basic_istream@DU?$char_traits@D@std@@@std@@AEAV01@PEAU__mpq_struct@@@Z");
//	p[2] = GetProcAddress(hL, "??5@YAAEAV?$basic_istream@DU?$char_traits@D@std@@@std@@AEAV01@PEAU__mpz_struct@@@Z");
//	p[3] = GetProcAddress(hL, "??6@YAAEAV?$basic_ostream@DU?$char_traits@D@std@@@std@@AEAV01@PEBU__mpf_struct@@@Z");
//	p[4] = GetProcAddress(hL, "??6@YAAEAV?$basic_ostream@DU?$char_traits@D@std@@@std@@AEAV01@PEBU__mpq_struct@@@Z");
//	p[5] = GetProcAddress(hL, "??6@YAAEAV?$basic_ostream@DU?$char_traits@D@std@@@std@@AEAV01@PEBU__mpz_struct@@@Z");
    p[6] = GetProcAddress(hL, "__combine_limbs");
    p[7] = GetProcAddress(hL, "__fermat_to_mpz");
    p[8] = GetProcAddress(hL, "__gmp_0");
    p[9] = GetProcAddress(hL, "__gmp_allocate_func");
    p[10] = GetProcAddress(hL, "__gmp_asprintf");
    p[11] = GetProcAddress(hL, "__gmp_asprintf_final");
    p[12] = GetProcAddress(hL, "__gmp_asprintf_memory");
    p[13] = GetProcAddress(hL, "__gmp_asprintf_reps");
    p[14] = GetProcAddress(hL, "__gmp_assert_fail");
    p[15] = GetProcAddress(hL, "__gmp_assert_header");
    p[16] = GetProcAddress(hL, "__gmp_bits_per_limb");
    p[17] = GetProcAddress(hL, "__gmp_default_allocate");
    p[18] = GetProcAddress(hL, "__gmp_default_fp_limb_precision");
    p[19] = GetProcAddress(hL, "__gmp_default_free");
    p[20] = GetProcAddress(hL, "__gmp_default_reallocate");
    p[21] = GetProcAddress(hL, "__gmp_digit_value_tab");
    p[22] = GetProcAddress(hL, "__gmp_divide_by_zero");
    p[23] = GetProcAddress(hL, "__gmp_doprnt");
    p[24] = GetProcAddress(hL, "__gmp_doprnt_integer");
    p[25] = GetProcAddress(hL, "__gmp_doprnt_mpf2");
    p[26] = GetProcAddress(hL, "__gmp_doscan");
    p[27] = GetProcAddress(hL, "__gmp_errno");
    p[28] = GetProcAddress(hL, "__gmp_exception");
    p[29] = GetProcAddress(hL, "__gmp_extract_double");
    p[30] = GetProcAddress(hL, "__gmp_fib_table");
    p[31] = GetProcAddress(hL, "__gmp_fprintf");
    p[32] = GetProcAddress(hL, "__gmp_free_func");
    p[33] = GetProcAddress(hL, "__gmp_fscanf");
    p[34] = GetProcAddress(hL, "__gmp_get_memory_functions");
    p[35] = GetProcAddress(hL, "__gmp_init_primesieve");
    p[36] = GetProcAddress(hL, "__gmp_invalid_operation");
    p[37] = GetProcAddress(hL, "__gmp_jacobi_table");
    p[38] = GetProcAddress(hL, "__gmp_junk");
    p[39] = GetProcAddress(hL, "__gmp_modlimb_invert_table");
    p[40] = GetProcAddress(hL, "__gmp_nextprime");
    p[41] = GetProcAddress(hL, "__gmp_primesieve");
    p[42] = GetProcAddress(hL, "__gmp_printf");
    p[43] = GetProcAddress(hL, "__gmp_randclear");
    p[44] = GetProcAddress(hL, "__gmp_randinit_default");
    p[45] = GetProcAddress(hL, "__gmp_randinit_lc_2exp");
    p[46] = GetProcAddress(hL, "__gmp_randinit_lc_2exp_size");
    p[47] = GetProcAddress(hL, "__gmp_randinit_mt");
    p[48] = GetProcAddress(hL, "__gmp_randinit_mt_noseed");
    p[49] = GetProcAddress(hL, "__gmp_randinit_set");
    p[50] = GetProcAddress(hL, "__gmp_rands");
    p[51] = GetProcAddress(hL, "__gmp_rands_initialized");
    p[52] = GetProcAddress(hL, "__gmp_randseed");
    p[53] = GetProcAddress(hL, "__gmp_randseed_ui");
    p[54] = GetProcAddress(hL, "__gmp_reallocate_func");
    p[55] = GetProcAddress(hL, "__gmp_replacement_vsnprintf");
    p[56] = GetProcAddress(hL, "__gmp_scanf");
    p[57] = GetProcAddress(hL, "__gmp_set_memory_functions");
    p[58] = GetProcAddress(hL, "__gmp_snprintf");
    p[59] = GetProcAddress(hL, "__gmp_sprintf");
    p[60] = GetProcAddress(hL, "__gmp_sqrt_of_negative");
    p[61] = GetProcAddress(hL, "__gmp_sscanf");
    p[62] = GetProcAddress(hL, "__gmp_tmp_reentrant_alloc");
    p[63] = GetProcAddress(hL, "__gmp_tmp_reentrant_free");
    p[64] = GetProcAddress(hL, "__gmp_urandomb_ui");
    p[65] = GetProcAddress(hL, "__gmp_urandomm_ui");
    p[66] = GetProcAddress(hL, "__gmp_vasprintf");
    p[67] = GetProcAddress(hL, "__gmp_version");
    p[68] = GetProcAddress(hL, "__gmp_vfprintf");
    p[69] = GetProcAddress(hL, "__gmp_vfscanf");
    p[70] = GetProcAddress(hL, "__gmp_vprintf");
    p[71] = GetProcAddress(hL, "__gmp_vscanf");
    p[72] = GetProcAddress(hL, "__gmp_vsnprintf");
    p[73] = GetProcAddress(hL, "__gmp_vsprintf");
    p[74] = GetProcAddress(hL, "__gmp_vsscanf");
    p[75] = GetProcAddress(hL, "__gmpf_abs");
    p[76] = GetProcAddress(hL, "__gmpf_add");
    p[77] = GetProcAddress(hL, "__gmpf_add_ui");
    p[78] = GetProcAddress(hL, "__gmpf_ceil");
    p[79] = GetProcAddress(hL, "__gmpf_clear");
    p[80] = GetProcAddress(hL, "__gmpf_clears");
    p[81] = GetProcAddress(hL, "__gmpf_cmp");
    p[82] = GetProcAddress(hL, "__gmpf_cmp_d");
    p[83] = GetProcAddress(hL, "__gmpf_cmp_si");
    p[84] = GetProcAddress(hL, "__gmpf_cmp_ui");
    p[85] = GetProcAddress(hL, "__gmpf_cmp_z");
    p[86] = GetProcAddress(hL, "__gmpf_div");
    p[87] = GetProcAddress(hL, "__gmpf_div_2exp");
    p[88] = GetProcAddress(hL, "__gmpf_div_ui");
    p[89] = GetProcAddress(hL, "__gmpf_dump");
    p[90] = GetProcAddress(hL, "__gmpf_eq");
    p[91] = GetProcAddress(hL, "__gmpf_fits_si_p");
    p[92] = GetProcAddress(hL, "__gmpf_fits_sint_p");
    p[93] = GetProcAddress(hL, "__gmpf_fits_slong_p");
    p[94] = GetProcAddress(hL, "__gmpf_fits_sshort_p");
    p[95] = GetProcAddress(hL, "__gmpf_fits_ui_p");
    p[96] = GetProcAddress(hL, "__gmpf_fits_uint_p");
    p[97] = GetProcAddress(hL, "__gmpf_fits_ulong_p");
    p[98] = GetProcAddress(hL, "__gmpf_fits_ushort_p");
    p[99] = GetProcAddress(hL, "__gmpf_floor");
    p[100] = GetProcAddress(hL, "__gmpf_get_2exp_d");
    p[101] = GetProcAddress(hL, "__gmpf_get_d");
    p[102] = GetProcAddress(hL, "__gmpf_get_d_2exp");
    p[103] = GetProcAddress(hL, "__gmpf_get_default_prec");
    p[104] = GetProcAddress(hL, "__gmpf_get_prec");
    p[105] = GetProcAddress(hL, "__gmpf_get_si");
    p[106] = GetProcAddress(hL, "__gmpf_get_str");
    p[107] = GetProcAddress(hL, "__gmpf_get_ui");
    p[108] = GetProcAddress(hL, "__gmpf_init");
    p[109] = GetProcAddress(hL, "__gmpf_init2");
    p[110] = GetProcAddress(hL, "__gmpf_init_set");
    p[111] = GetProcAddress(hL, "__gmpf_init_set_d");
    p[112] = GetProcAddress(hL, "__gmpf_init_set_si");
    p[113] = GetProcAddress(hL, "__gmpf_init_set_str");
    p[114] = GetProcAddress(hL, "__gmpf_init_set_ui");
    p[115] = GetProcAddress(hL, "__gmpf_inits");
    p[116] = GetProcAddress(hL, "__gmpf_inp_str");
    p[117] = GetProcAddress(hL, "__gmpf_integer_p");
    p[118] = GetProcAddress(hL, "__gmpf_mul");
    p[119] = GetProcAddress(hL, "__gmpf_mul_2exp");
    p[120] = GetProcAddress(hL, "__gmpf_mul_ui");
    p[121] = GetProcAddress(hL, "__gmpf_neg");
    p[122] = GetProcAddress(hL, "__gmpf_out_str");
    p[123] = GetProcAddress(hL, "__gmpf_pow_ui");
    p[124] = GetProcAddress(hL, "__gmpf_random2");
    p[125] = GetProcAddress(hL, "__gmpf_reldiff");
    p[126] = GetProcAddress(hL, "__gmpf_rrandomb");
    p[127] = GetProcAddress(hL, "__gmpf_set");
    p[128] = GetProcAddress(hL, "__gmpf_set_d");
    p[129] = GetProcAddress(hL, "__gmpf_set_default_prec");
    p[130] = GetProcAddress(hL, "__gmpf_set_prec");
    p[131] = GetProcAddress(hL, "__gmpf_set_prec_raw");
    p[132] = GetProcAddress(hL, "__gmpf_set_q");
    p[133] = GetProcAddress(hL, "__gmpf_set_si");
    p[134] = GetProcAddress(hL, "__gmpf_set_str");
    p[135] = GetProcAddress(hL, "__gmpf_set_ui");
    p[136] = GetProcAddress(hL, "__gmpf_set_z");
    p[137] = GetProcAddress(hL, "__gmpf_size");
    p[138] = GetProcAddress(hL, "__gmpf_sqrt");
    p[139] = GetProcAddress(hL, "__gmpf_sqrt_ui");
    p[140] = GetProcAddress(hL, "__gmpf_sub");
    p[141] = GetProcAddress(hL, "__gmpf_sub_ui");
    p[142] = GetProcAddress(hL, "__gmpf_swap");
    p[143] = GetProcAddress(hL, "__gmpf_trunc");
    p[144] = GetProcAddress(hL, "__gmpf_ui_div");
    p[145] = GetProcAddress(hL, "__gmpf_ui_sub");
    p[146] = GetProcAddress(hL, "__gmpf_urandomb");
    p[147] = GetProcAddress(hL, "__gmpn_add");
    p[148] = GetProcAddress(hL, "__gmpn_add_1");
    p[149] = GetProcAddress(hL, "__gmpn_add_err1_n");
    p[150] = GetProcAddress(hL, "__gmpn_add_err2_n");
    p[151] = GetProcAddress(hL, "__gmpn_add_n");
    p[152] = GetProcAddress(hL, "__gmpn_addadd_n");
    p[153] = GetProcAddress(hL, "__gmpn_addmul_1");
    p[154] = GetProcAddress(hL, "__gmpn_addmul_2");
    p[155] = GetProcAddress(hL, "__gmpn_addsub_n");
    p[156] = GetProcAddress(hL, "__gmpn_and_n");
    p[157] = GetProcAddress(hL, "__gmpn_andn_n");
    p[158] = GetProcAddress(hL, "__gmpn_bases");
    p[159] = GetProcAddress(hL, "__gmpn_bc_set_str");
    p[160] = GetProcAddress(hL, "__gmpn_bdivmod");
    p[161] = GetProcAddress(hL, "__gmpn_binvert");
    p[162] = GetProcAddress(hL, "__gmpn_binvert_itch");
    p[163] = GetProcAddress(hL, "__gmpn_clz_tab");
    p[164] = GetProcAddress(hL, "__gmpn_cmp");
    p[165] = GetProcAddress(hL, "__gmpn_com_n");
    p[166] = GetProcAddress(hL, "__gmpn_copyd");
    p[167] = GetProcAddress(hL, "__gmpn_copyi");
    p[168] = GetProcAddress(hL, "__gmpn_dc_bdiv_q");
    p[169] = GetProcAddress(hL, "__gmpn_dc_bdiv_q_n");
    p[170] = GetProcAddress(hL, "__gmpn_dc_bdiv_qr");
    p[171] = GetProcAddress(hL, "__gmpn_dc_bdiv_qr_n");
    p[172] = GetProcAddress(hL, "__gmpn_dc_div_q");
    p[173] = GetProcAddress(hL, "__gmpn_dc_div_qr");
    p[174] = GetProcAddress(hL, "__gmpn_dc_div_qr_n");
    p[175] = GetProcAddress(hL, "__gmpn_dc_divappr_q");
    p[176] = GetProcAddress(hL, "__gmpn_dc_set_str");
    p[177] = GetProcAddress(hL, "__gmpn_div_2expmod_2expp1");
    p[178] = GetProcAddress(hL, "__gmpn_divexact");
    p[179] = GetProcAddress(hL, "__gmpn_divexact_1");
    p[180] = GetProcAddress(hL, "__gmpn_divexact_by3c");
    p[181] = GetProcAddress(hL, "__gmpn_divexact_byff");
    p[182] = GetProcAddress(hL, "__gmpn_divexact_byfobm1");
    p[183] = GetProcAddress(hL, "__gmpn_divisible_p");
    p[184] = GetProcAddress(hL, "__gmpn_divrem");
    p[185] = GetProcAddress(hL, "__gmpn_divrem_1");
    p[186] = GetProcAddress(hL, "__gmpn_divrem_2");
    p[187] = GetProcAddress(hL, "__gmpn_divrem_euclidean_qr_1");
    p[188] = GetProcAddress(hL, "__gmpn_divrem_euclidean_qr_2");
    p[189] = GetProcAddress(hL, "__gmpn_divrem_euclidean_r_1");
    p[190] = GetProcAddress(hL, "__gmpn_divrem_hensel_qr_1");
    p[191] = GetProcAddress(hL, "__gmpn_divrem_hensel_qr_1_1");
    p[192] = GetProcAddress(hL, "__gmpn_divrem_hensel_qr_1_2");
    p[193] = GetProcAddress(hL, "__gmpn_divrem_hensel_r_1");
    p[194] = GetProcAddress(hL, "__gmpn_divrem_hensel_rsh_qr_1");
    p[195] = GetProcAddress(hL, "__gmpn_divrem_hensel_rsh_qr_1_preinv");
    p[196] = GetProcAddress(hL, "__gmpn_dump");
    p[197] = GetProcAddress(hL, "__gmpn_fib2_ui");
    p[198] = GetProcAddress(hL, "__gmpn_gcd");
    p[199] = GetProcAddress(hL, "__gmpn_gcd_1");
    p[200] = GetProcAddress(hL, "__gmpn_gcd_subdiv_step");
    p[201] = GetProcAddress(hL, "__gmpn_gcdext");
    p[202] = GetProcAddress(hL, "__gmpn_gcdext_1");
    p[203] = GetProcAddress(hL, "__gmpn_gcdext_hook");
    p[204] = GetProcAddress(hL, "__gmpn_gcdext_lehmer_n");
    p[205] = GetProcAddress(hL, "__gmpn_get_d");
    p[206] = GetProcAddress(hL, "__gmpn_get_str");
    p[207] = GetProcAddress(hL, "__gmpn_hamdist");
    p[208] = GetProcAddress(hL, "__gmpn_hgcd");
    p[209] = GetProcAddress(hL, "__gmpn_hgcd2");
    p[210] = GetProcAddress(hL, "__gmpn_hgcd2_jacobi");
    p[211] = GetProcAddress(hL, "__gmpn_hgcd_appr");
    p[212] = GetProcAddress(hL, "__gmpn_hgcd_appr_itch");
    p[213] = GetProcAddress(hL, "__gmpn_hgcd_itch");
    p[214] = GetProcAddress(hL, "__gmpn_hgcd_jacobi");
    p[215] = GetProcAddress(hL, "__gmpn_hgcd_matrix_adjust");
    p[216] = GetProcAddress(hL, "__gmpn_hgcd_matrix_init");
    p[217] = GetProcAddress(hL, "__gmpn_hgcd_matrix_mul");
    p[218] = GetProcAddress(hL, "__gmpn_hgcd_matrix_mul_1");
    p[219] = GetProcAddress(hL, "__gmpn_hgcd_matrix_update_q");
    p[220] = GetProcAddress(hL, "__gmpn_hgcd_mul_matrix1_vector");
    p[221] = GetProcAddress(hL, "__gmpn_hgcd_reduce");
    p[222] = GetProcAddress(hL, "__gmpn_hgcd_reduce_itch");
    p[223] = GetProcAddress(hL, "__gmpn_hgcd_step");
    p[224] = GetProcAddress(hL, "__gmpn_inv_div_q");
    p[225] = GetProcAddress(hL, "__gmpn_inv_div_qr");
    p[226] = GetProcAddress(hL, "__gmpn_inv_div_qr_n");
    p[227] = GetProcAddress(hL, "__gmpn_inv_divappr_q");
    p[228] = GetProcAddress(hL, "__gmpn_inv_divappr_q_n");
    p[229] = GetProcAddress(hL, "__gmpn_invert");
    p[230] = GetProcAddress(hL, "__gmpn_invert_trunc");
    p[231] = GetProcAddress(hL, "__gmpn_ior_n");
    p[232] = GetProcAddress(hL, "__gmpn_iorn_n");
    p[233] = GetProcAddress(hL, "__gmpn_is_invert");
    p[234] = GetProcAddress(hL, "__gmpn_jacobi_2");
    p[235] = GetProcAddress(hL, "__gmpn_jacobi_base");
    p[236] = GetProcAddress(hL, "__gmpn_jacobi_n");
    p[237] = GetProcAddress(hL, "__gmpn_kara_mul_n");
    p[238] = GetProcAddress(hL, "__gmpn_kara_sqr_n");
    p[239] = GetProcAddress(hL, "__gmpn_lshift");
    p[240] = GetProcAddress(hL, "__gmpn_matrix22_mul");
    p[241] = GetProcAddress(hL, "__gmpn_matrix22_mul1_inverse_vector");
    p[242] = GetProcAddress(hL, "__gmpn_matrix22_mul_itch");
    p[243] = GetProcAddress(hL, "__gmpn_matrix22_mul_strassen");
    p[244] = GetProcAddress(hL, "__gmpn_mod_1");
    p[245] = GetProcAddress(hL, "__gmpn_mod_1_1");
    p[246] = GetProcAddress(hL, "__gmpn_mod_1_2");
    p[247] = GetProcAddress(hL, "__gmpn_mod_1_3");
    p[248] = GetProcAddress(hL, "__gmpn_mod_1_k");
    p[249] = GetProcAddress(hL, "__gmpn_mod_34lsub1");
    p[250] = GetProcAddress(hL, "__gmpn_modexact_1c_odd");
    p[251] = GetProcAddress(hL, "__gmpn_mul");
    p[252] = GetProcAddress(hL, "__gmpn_mul_1");
    p[253] = GetProcAddress(hL, "__gmpn_mul_2expmod_2expp1");
    p[254] = GetProcAddress(hL, "__gmpn_mul_basecase");
    p[255] = GetProcAddress(hL, "__gmpn_mul_fft");
    p[256] = GetProcAddress(hL, "__gmpn_mul_fft_main");
    p[257] = GetProcAddress(hL, "__gmpn_mul_mfa_trunc_sqrt2");
    p[258] = GetProcAddress(hL, "__gmpn_mul_n");
    p[259] = GetProcAddress(hL, "__gmpn_mul_trunc_sqrt2");
    p[260] = GetProcAddress(hL, "__gmpn_mulhigh_n");
    p[261] = GetProcAddress(hL, "__gmpn_mullow_basecase");
    p[262] = GetProcAddress(hL, "__gmpn_mullow_n");
    p[263] = GetProcAddress(hL, "__gmpn_mullow_n_basecase");
    p[264] = GetProcAddress(hL, "__gmpn_mulmid");
    p[265] = GetProcAddress(hL, "__gmpn_mulmid_basecase");
    p[266] = GetProcAddress(hL, "__gmpn_mulmid_n");
    p[267] = GetProcAddress(hL, "__gmpn_mulmod_2expm1");
    p[268] = GetProcAddress(hL, "__gmpn_mulmod_2expp1_basecase");
    p[269] = GetProcAddress(hL, "__gmpn_mulmod_Bexpp1");
    p[270] = GetProcAddress(hL, "__gmpn_mulmod_Bexpp1_fft");
    p[271] = GetProcAddress(hL, "__gmpn_mulmod_bnm1");
    p[272] = GetProcAddress(hL, "__gmpn_nand_n");
    p[273] = GetProcAddress(hL, "__gmpn_nior_n");
    p[274] = GetProcAddress(hL, "__gmpn_normmod_2expp1");
    p[275] = GetProcAddress(hL, "__gmpn_nsumdiff_n");
    p[276] = GetProcAddress(hL, "__gmpn_perfect_square_p");
    p[277] = GetProcAddress(hL, "__gmpn_popcount");
    p[278] = GetProcAddress(hL, "__gmpn_pow_1");
    p[279] = GetProcAddress(hL, "__gmpn_powlo");
    p[280] = GetProcAddress(hL, "__gmpn_powm");
    p[281] = GetProcAddress(hL, "__gmpn_preinv_divrem_1");
    p[282] = GetProcAddress(hL, "__gmpn_preinv_mod_1");
    p[283] = GetProcAddress(hL, "__gmpn_random");
    p[284] = GetProcAddress(hL, "__gmpn_random2");
    p[285] = GetProcAddress(hL, "__gmpn_randomb");
    p[286] = GetProcAddress(hL, "__gmpn_redc_1");
    p[287] = GetProcAddress(hL, "__gmpn_redc_2");
    p[288] = GetProcAddress(hL, "__gmpn_redc_n");
    p[289] = GetProcAddress(hL, "__gmpn_rootrem");
    p[290] = GetProcAddress(hL, "__gmpn_rootrem_basecase");
    p[291] = GetProcAddress(hL, "__gmpn_rrandom");
    p[292] = GetProcAddress(hL, "__gmpn_rsh_divrem_hensel_qr_1");
    p[293] = GetProcAddress(hL, "__gmpn_rsh_divrem_hensel_qr_1_1");
    p[294] = GetProcAddress(hL, "__gmpn_rsh_divrem_hensel_qr_1_2");
    p[295] = GetProcAddress(hL, "__gmpn_rshift");
    p[296] = GetProcAddress(hL, "__gmpn_sb_bdiv_q");
    p[297] = GetProcAddress(hL, "__gmpn_sb_bdiv_qr");
    p[298] = GetProcAddress(hL, "__gmpn_sb_div_q");
    p[299] = GetProcAddress(hL, "__gmpn_sb_div_qr");
    p[300] = GetProcAddress(hL, "__gmpn_sb_divappr_q");
    p[301] = GetProcAddress(hL, "__gmpn_scan0");
    p[302] = GetProcAddress(hL, "__gmpn_scan1");
    p[303] = GetProcAddress(hL, "__gmpn_set_str");
    p[304] = GetProcAddress(hL, "__gmpn_set_str_compute_powtab");
    p[305] = GetProcAddress(hL, "__gmpn_sizeinbase");
    p[306] = GetProcAddress(hL, "__gmpn_sqr");
    p[307] = GetProcAddress(hL, "__gmpn_sqr_basecase");
    p[308] = GetProcAddress(hL, "__gmpn_sqrtrem");
    p[309] = GetProcAddress(hL, "__gmpn_sub");
    p[310] = GetProcAddress(hL, "__gmpn_sub_1");
    p[311] = GetProcAddress(hL, "__gmpn_sub_err1_n");
    p[312] = GetProcAddress(hL, "__gmpn_sub_err2_n");
    p[313] = GetProcAddress(hL, "__gmpn_sub_n");
    p[314] = GetProcAddress(hL, "__gmpn_subadd_n");
    p[315] = GetProcAddress(hL, "__gmpn_submul_1");
    p[316] = GetProcAddress(hL, "__gmpn_sumdiff_n");
    p[317] = GetProcAddress(hL, "__gmpn_tdiv_q");
    p[318] = GetProcAddress(hL, "__gmpn_tdiv_qr");
    p[319] = GetProcAddress(hL, "__gmpn_toom32_mul");
    p[320] = GetProcAddress(hL, "__gmpn_toom3_interpolate");
    p[321] = GetProcAddress(hL, "__gmpn_toom3_mul");
    p[322] = GetProcAddress(hL, "__gmpn_toom3_mul_n");
    p[323] = GetProcAddress(hL, "__gmpn_toom3_sqr_n");
    p[324] = GetProcAddress(hL, "__gmpn_toom42_mul");
    p[325] = GetProcAddress(hL, "__gmpn_toom42_mulmid");
    p[326] = GetProcAddress(hL, "__gmpn_toom4_interpolate");
    p[327] = GetProcAddress(hL, "__gmpn_toom4_mul");
    p[328] = GetProcAddress(hL, "__gmpn_toom4_mul_n");
    p[329] = GetProcAddress(hL, "__gmpn_toom4_sqr_n");
    p[330] = GetProcAddress(hL, "__gmpn_toom53_mul");
    p[331] = GetProcAddress(hL, "__gmpn_toom8_sqr_n");
    p[332] = GetProcAddress(hL, "__gmpn_toom8h_mul");
    p[333] = GetProcAddress(hL, "__gmpn_toom_couple_handling");
    p[334] = GetProcAddress(hL, "__gmpn_toom_eval_dgr3_pm1");
    p[335] = GetProcAddress(hL, "__gmpn_toom_eval_dgr3_pm2");
    p[336] = GetProcAddress(hL, "__gmpn_toom_eval_pm1");
    p[337] = GetProcAddress(hL, "__gmpn_toom_eval_pm2");
    p[338] = GetProcAddress(hL, "__gmpn_toom_eval_pm2exp");
    p[339] = GetProcAddress(hL, "__gmpn_toom_eval_pm2rexp");
    p[340] = GetProcAddress(hL, "__gmpn_toom_interpolate_16pts");
    p[341] = GetProcAddress(hL, "__gmpn_urandomb");
    p[342] = GetProcAddress(hL, "__gmpn_urandomm");
    p[343] = GetProcAddress(hL, "__gmpn_xnor_n");
    p[344] = GetProcAddress(hL, "__gmpn_xor_n");
    p[345] = GetProcAddress(hL, "__gmpn_zero");
    p[346] = GetProcAddress(hL, "__gmpn_zero_p");
    p[347] = GetProcAddress(hL, "__gmpq_abs");
    p[348] = GetProcAddress(hL, "__gmpq_add");
    p[349] = GetProcAddress(hL, "__gmpq_canonicalize");
    p[350] = GetProcAddress(hL, "__gmpq_clear");
    p[351] = GetProcAddress(hL, "__gmpq_clears");
    p[352] = GetProcAddress(hL, "__gmpq_cmp");
    p[353] = GetProcAddress(hL, "__gmpq_cmp_si");
    p[354] = GetProcAddress(hL, "__gmpq_cmp_ui");
    p[355] = GetProcAddress(hL, "__gmpq_cmp_z");
    p[356] = GetProcAddress(hL, "__gmpq_div");
    p[357] = GetProcAddress(hL, "__gmpq_div_2exp");
    p[358] = GetProcAddress(hL, "__gmpq_equal");
    p[359] = GetProcAddress(hL, "__gmpq_get_d");
    p[360] = GetProcAddress(hL, "__gmpq_get_den");
    p[361] = GetProcAddress(hL, "__gmpq_get_num");
    p[362] = GetProcAddress(hL, "__gmpq_get_str");
    p[363] = GetProcAddress(hL, "__gmpq_init");
    p[364] = GetProcAddress(hL, "__gmpq_inits");
    p[365] = GetProcAddress(hL, "__gmpq_inp_str");
    p[366] = GetProcAddress(hL, "__gmpq_inv");
    p[367] = GetProcAddress(hL, "__gmpq_mul");
    p[368] = GetProcAddress(hL, "__gmpq_mul_2exp");
    p[369] = GetProcAddress(hL, "__gmpq_neg");
    p[370] = GetProcAddress(hL, "__gmpq_out_str");
    p[371] = GetProcAddress(hL, "__gmpq_set");
    p[372] = GetProcAddress(hL, "__gmpq_set_d");
    p[373] = GetProcAddress(hL, "__gmpq_set_den");
    p[374] = GetProcAddress(hL, "__gmpq_set_f");
    p[375] = GetProcAddress(hL, "__gmpq_set_num");
    p[376] = GetProcAddress(hL, "__gmpq_set_si");
    p[377] = GetProcAddress(hL, "__gmpq_set_str");
    p[378] = GetProcAddress(hL, "__gmpq_set_ui");
    p[379] = GetProcAddress(hL, "__gmpq_set_z");
    p[380] = GetProcAddress(hL, "__gmpq_sub");
    p[381] = GetProcAddress(hL, "__gmpq_swap");
    p[382] = GetProcAddress(hL, "__gmpz_2fac_ui");
    p[383] = GetProcAddress(hL, "__gmpz_abs");
    p[384] = GetProcAddress(hL, "__gmpz_add");
    p[385] = GetProcAddress(hL, "__gmpz_add_ui");
    p[386] = GetProcAddress(hL, "__gmpz_addmul");
    p[387] = GetProcAddress(hL, "__gmpz_addmul_ui");
    p[388] = GetProcAddress(hL, "__gmpz_and");
    p[389] = GetProcAddress(hL, "__gmpz_aorsmul_1");
    p[390] = GetProcAddress(hL, "__gmpz_array_init");
    p[391] = GetProcAddress(hL, "__gmpz_bin_ui");
    p[392] = GetProcAddress(hL, "__gmpz_bin_uiui");
    p[393] = GetProcAddress(hL, "__gmpz_cdiv_q");
    p[394] = GetProcAddress(hL, "__gmpz_cdiv_q_2exp");
    p[395] = GetProcAddress(hL, "__gmpz_cdiv_q_ui");
    p[396] = GetProcAddress(hL, "__gmpz_cdiv_qr");
    p[397] = GetProcAddress(hL, "__gmpz_cdiv_qr_ui");
    p[398] = GetProcAddress(hL, "__gmpz_cdiv_r");
    p[399] = GetProcAddress(hL, "__gmpz_cdiv_r_2exp");
    p[400] = GetProcAddress(hL, "__gmpz_cdiv_r_ui");
    p[401] = GetProcAddress(hL, "__gmpz_cdiv_ui");
    p[402] = GetProcAddress(hL, "__gmpz_clear");
    p[403] = GetProcAddress(hL, "__gmpz_clears");
    p[404] = GetProcAddress(hL, "__gmpz_clrbit");
    p[405] = GetProcAddress(hL, "__gmpz_cmp");
    p[406] = GetProcAddress(hL, "__gmpz_cmp_d");
    p[407] = GetProcAddress(hL, "__gmpz_cmp_si");
    p[408] = GetProcAddress(hL, "__gmpz_cmp_ui");
    p[409] = GetProcAddress(hL, "__gmpz_cmpabs");
    p[410] = GetProcAddress(hL, "__gmpz_cmpabs_d");
    p[411] = GetProcAddress(hL, "__gmpz_cmpabs_ui");
    p[412] = GetProcAddress(hL, "__gmpz_com");
    p[413] = GetProcAddress(hL, "__gmpz_combit");
    p[414] = GetProcAddress(hL, "__gmpz_congruent_2exp_p");
    p[415] = GetProcAddress(hL, "__gmpz_congruent_p");
    p[416] = GetProcAddress(hL, "__gmpz_congruent_ui_p");
    p[417] = GetProcAddress(hL, "__gmpz_divexact");
    p[418] = GetProcAddress(hL, "__gmpz_divexact_gcd");
    p[419] = GetProcAddress(hL, "__gmpz_divexact_ui");
    p[420] = GetProcAddress(hL, "__gmpz_divisible_2exp_p");
    p[421] = GetProcAddress(hL, "__gmpz_divisible_p");
    p[422] = GetProcAddress(hL, "__gmpz_divisible_ui_p");
    p[423] = GetProcAddress(hL, "__gmpz_dump");
    p[424] = GetProcAddress(hL, "__gmpz_export");
    p[425] = GetProcAddress(hL, "__gmpz_fac_ui");
    p[426] = GetProcAddress(hL, "__gmpz_fdiv_q");
    p[427] = GetProcAddress(hL, "__gmpz_fdiv_q_2exp");
    p[428] = GetProcAddress(hL, "__gmpz_fdiv_q_ui");
    p[429] = GetProcAddress(hL, "__gmpz_fdiv_qr");
    p[430] = GetProcAddress(hL, "__gmpz_fdiv_qr_ui");
    p[431] = GetProcAddress(hL, "__gmpz_fdiv_r");
    p[432] = GetProcAddress(hL, "__gmpz_fdiv_r_2exp");
    p[433] = GetProcAddress(hL, "__gmpz_fdiv_r_ui");
    p[434] = GetProcAddress(hL, "__gmpz_fdiv_ui");
    p[435] = GetProcAddress(hL, "__gmpz_fib2_ui");
    p[436] = GetProcAddress(hL, "__gmpz_fib_ui");
    p[437] = GetProcAddress(hL, "__gmpz_fits_si_p");
    p[438] = GetProcAddress(hL, "__gmpz_fits_sint_p");
    p[439] = GetProcAddress(hL, "__gmpz_fits_slong_p");
    p[440] = GetProcAddress(hL, "__gmpz_fits_sshort_p");
    p[441] = GetProcAddress(hL, "__gmpz_fits_ui_p");
    p[442] = GetProcAddress(hL, "__gmpz_fits_uint_p");
    p[443] = GetProcAddress(hL, "__gmpz_fits_ulong_p");
    p[444] = GetProcAddress(hL, "__gmpz_fits_ushort_p");
    p[445] = GetProcAddress(hL, "__gmpz_gcd");
    p[446] = GetProcAddress(hL, "__gmpz_gcd_ui");
    p[447] = GetProcAddress(hL, "__gmpz_gcdext");
    p[448] = GetProcAddress(hL, "__gmpz_get_2exp_d");
    p[449] = GetProcAddress(hL, "__gmpz_get_d");
    p[450] = GetProcAddress(hL, "__gmpz_get_d_2exp");
    p[451] = GetProcAddress(hL, "__gmpz_get_si");
    p[452] = GetProcAddress(hL, "__gmpz_get_str");
    p[453] = GetProcAddress(hL, "__gmpz_get_sx");
    p[454] = GetProcAddress(hL, "__gmpz_get_ui");
    p[455] = GetProcAddress(hL, "__gmpz_get_ux");
    p[456] = GetProcAddress(hL, "__gmpz_getlimbn");
    p[457] = GetProcAddress(hL, "__gmpz_hamdist");
    p[458] = GetProcAddress(hL, "__gmpz_import");
    p[459] = GetProcAddress(hL, "__gmpz_init");
    p[460] = GetProcAddress(hL, "__gmpz_init2");
    p[461] = GetProcAddress(hL, "__gmpz_init_set");
    p[462] = GetProcAddress(hL, "__gmpz_init_set_d");
    p[463] = GetProcAddress(hL, "__gmpz_init_set_si");
    p[464] = GetProcAddress(hL, "__gmpz_init_set_str");
    p[465] = GetProcAddress(hL, "__gmpz_init_set_sx");
    p[466] = GetProcAddress(hL, "__gmpz_init_set_ui");
    p[467] = GetProcAddress(hL, "__gmpz_init_set_ux");
    p[468] = GetProcAddress(hL, "__gmpz_inits");
    p[469] = GetProcAddress(hL, "__gmpz_inp_raw");
    p[470] = GetProcAddress(hL, "__gmpz_inp_str");
    p[471] = GetProcAddress(hL, "__gmpz_inp_str_nowhite");
    p[472] = GetProcAddress(hL, "__gmpz_invert");
    p[473] = GetProcAddress(hL, "__gmpz_ior");
    p[474] = GetProcAddress(hL, "__gmpz_jacobi");
    p[475] = GetProcAddress(hL, "__gmpz_kronecker_si");
    p[476] = GetProcAddress(hL, "__gmpz_kronecker_ui");
    p[477] = GetProcAddress(hL, "__gmpz_lcm");
    p[478] = GetProcAddress(hL, "__gmpz_lcm_ui");
    p[479] = GetProcAddress(hL, "__gmpz_likely_prime_p");
    p[480] = GetProcAddress(hL, "__gmpz_limbs_finish");
    p[481] = GetProcAddress(hL, "__gmpz_limbs_modify");
    p[482] = GetProcAddress(hL, "__gmpz_limbs_read");
    p[483] = GetProcAddress(hL, "__gmpz_limbs_write");
    p[484] = GetProcAddress(hL, "__gmpz_lucnum2_ui");
    p[485] = GetProcAddress(hL, "__gmpz_lucnum_ui");
    p[486] = GetProcAddress(hL, "__gmpz_mfac_uiui");
    p[487] = GetProcAddress(hL, "__gmpz_miller_rabin");
    p[488] = GetProcAddress(hL, "__gmpz_millerrabin");
    p[489] = GetProcAddress(hL, "__gmpz_mod");
    p[490] = GetProcAddress(hL, "__gmpz_mul");
    p[491] = GetProcAddress(hL, "__gmpz_mul_2exp");
    p[492] = GetProcAddress(hL, "__gmpz_mul_si");
    p[493] = GetProcAddress(hL, "__gmpz_mul_ui");
    p[494] = GetProcAddress(hL, "__gmpz_n_pow_ui");
    p[495] = GetProcAddress(hL, "__gmpz_neg");
    p[496] = GetProcAddress(hL, "__gmpz_next_prime_candidate");
    p[497] = GetProcAddress(hL, "__gmpz_nextprime");
    p[498] = GetProcAddress(hL, "__gmpz_nthroot");
    p[499] = GetProcAddress(hL, "__gmpz_oddfac_1");
    p[500] = GetProcAddress(hL, "__gmpz_out_raw");
    p[501] = GetProcAddress(hL, "__gmpz_out_str");
    p[502] = GetProcAddress(hL, "__gmpz_perfect_power_p");
    p[503] = GetProcAddress(hL, "__gmpz_perfect_square_p");
    p[504] = GetProcAddress(hL, "__gmpz_popcount");
    p[505] = GetProcAddress(hL, "__gmpz_pow_ui");
    p[506] = GetProcAddress(hL, "__gmpz_powm");
    p[507] = GetProcAddress(hL, "__gmpz_powm_ui");
    p[508] = GetProcAddress(hL, "__gmpz_primorial_ui");
    p[509] = GetProcAddress(hL, "__gmpz_probab_prime_p");
    p[510] = GetProcAddress(hL, "__gmpz_probable_prime_p");
    p[511] = GetProcAddress(hL, "__gmpz_prodlimbs");
    p[512] = GetProcAddress(hL, "__gmpz_realloc");
    p[513] = GetProcAddress(hL, "__gmpz_realloc2");
    p[514] = GetProcAddress(hL, "__gmpz_remove");
    p[515] = GetProcAddress(hL, "__gmpz_roinit_n");
    p[516] = GetProcAddress(hL, "__gmpz_root");
    p[517] = GetProcAddress(hL, "__gmpz_rootrem");
    p[518] = GetProcAddress(hL, "__gmpz_rrandomb");
    p[519] = GetProcAddress(hL, "__gmpz_scan0");
    p[520] = GetProcAddress(hL, "__gmpz_scan1");
    p[521] = GetProcAddress(hL, "__gmpz_set");
    p[522] = GetProcAddress(hL, "__gmpz_set_d");
    p[523] = GetProcAddress(hL, "__gmpz_set_f");
    p[524] = GetProcAddress(hL, "__gmpz_set_q");
    p[525] = GetProcAddress(hL, "__gmpz_set_si");
    p[526] = GetProcAddress(hL, "__gmpz_set_str");
    p[527] = GetProcAddress(hL, "__gmpz_set_sx");
    p[528] = GetProcAddress(hL, "__gmpz_set_ui");
    p[529] = GetProcAddress(hL, "__gmpz_set_ux");
    p[530] = GetProcAddress(hL, "__gmpz_setbit");
    p[531] = GetProcAddress(hL, "__gmpz_si_kronecker");
    p[532] = GetProcAddress(hL, "__gmpz_size");
    p[533] = GetProcAddress(hL, "__gmpz_sizeinbase");
    p[534] = GetProcAddress(hL, "__gmpz_sqrt");
    p[535] = GetProcAddress(hL, "__gmpz_sqrtrem");
    p[536] = GetProcAddress(hL, "__gmpz_sub");
    p[537] = GetProcAddress(hL, "__gmpz_sub_ui");
    p[538] = GetProcAddress(hL, "__gmpz_submul");
    p[539] = GetProcAddress(hL, "__gmpz_submul_ui");
    p[540] = GetProcAddress(hL, "__gmpz_swap");
    p[541] = GetProcAddress(hL, "__gmpz_tdiv_q");
    p[542] = GetProcAddress(hL, "__gmpz_tdiv_q_2exp");
    p[543] = GetProcAddress(hL, "__gmpz_tdiv_q_ui");
    p[544] = GetProcAddress(hL, "__gmpz_tdiv_qr");
    p[545] = GetProcAddress(hL, "__gmpz_tdiv_qr_ui");
    p[546] = GetProcAddress(hL, "__gmpz_tdiv_r");
    p[547] = GetProcAddress(hL, "__gmpz_tdiv_r_2exp");
    p[548] = GetProcAddress(hL, "__gmpz_tdiv_r_ui");
    p[549] = GetProcAddress(hL, "__gmpz_tdiv_ui");
    p[550] = GetProcAddress(hL, "__gmpz_trial_division");
    p[551] = GetProcAddress(hL, "__gmpz_tstbit");
    p[552] = GetProcAddress(hL, "__gmpz_ui_kronecker");
    p[553] = GetProcAddress(hL, "__gmpz_ui_pow_ui");
    p[554] = GetProcAddress(hL, "__gmpz_ui_sub");
    p[555] = GetProcAddress(hL, "__gmpz_urandomb");
    p[556] = GetProcAddress(hL, "__gmpz_urandomm");
    p[557] = GetProcAddress(hL, "__gmpz_xor");
    p[558] = GetProcAddress(hL, "__mpir_butterfly_lshB");
    p[559] = GetProcAddress(hL, "__mpir_butterfly_rshB");
    p[560] = GetProcAddress(hL, "__mpir_fft_adjust");
    p[561] = GetProcAddress(hL, "__mpir_fft_adjust_limbs");
    p[562] = GetProcAddress(hL, "__mpir_fft_adjust_sqrt2");
    p[563] = GetProcAddress(hL, "__mpir_fft_butterfly");
    p[564] = GetProcAddress(hL, "__mpir_fft_butterfly_sqrt2");
    p[565] = GetProcAddress(hL, "__mpir_fft_butterfly_twiddle");
    p[566] = GetProcAddress(hL, "__mpir_fft_combine_bits");
    p[567] = GetProcAddress(hL, "__mpir_fft_mfa_trunc_sqrt2");
    p[568] = GetProcAddress(hL, "__mpir_fft_mfa_trunc_sqrt2_inner");
    p[569] = GetProcAddress(hL, "__mpir_fft_mfa_trunc_sqrt2_outer");
    p[570] = GetProcAddress(hL, "__mpir_fft_mulmod_2expp1");
    p[571] = GetProcAddress(hL, "__mpir_fft_naive_convolution_1");
    p[572] = GetProcAddress(hL, "__mpir_fft_negacyclic");
    p[573] = GetProcAddress(hL, "__mpir_fft_radix2");
    p[574] = GetProcAddress(hL, "__mpir_fft_radix2_twiddle");
    p[575] = GetProcAddress(hL, "__mpir_fft_split_bits");
    p[576] = GetProcAddress(hL, "__mpir_fft_split_limbs");
    p[577] = GetProcAddress(hL, "__mpir_fft_trunc");
    p[578] = GetProcAddress(hL, "__mpir_fft_trunc1");
    p[579] = GetProcAddress(hL, "__mpir_fft_trunc1_twiddle");
    p[580] = GetProcAddress(hL, "__mpir_fft_trunc_sqrt2");
    p[581] = GetProcAddress(hL, "__mpir_ifft_butterfly");
    p[582] = GetProcAddress(hL, "__mpir_ifft_butterfly_sqrt2");
    p[583] = GetProcAddress(hL, "__mpir_ifft_butterfly_twiddle");
    p[584] = GetProcAddress(hL, "__mpir_ifft_mfa_trunc_sqrt2");
    p[585] = GetProcAddress(hL, "__mpir_ifft_mfa_trunc_sqrt2_outer");
    p[586] = GetProcAddress(hL, "__mpir_ifft_negacyclic");
    p[587] = GetProcAddress(hL, "__mpir_ifft_radix2");
    p[588] = GetProcAddress(hL, "__mpir_ifft_radix2_twiddle");
    p[589] = GetProcAddress(hL, "__mpir_ifft_trunc");
    p[590] = GetProcAddress(hL, "__mpir_ifft_trunc1");
    p[591] = GetProcAddress(hL, "__mpir_ifft_trunc1_twiddle");
    p[592] = GetProcAddress(hL, "__mpir_ifft_trunc_sqrt2");
    p[593] = GetProcAddress(hL, "__mpir_revbin");
    p[594] = GetProcAddress(hL, "__mpir_version");
    p[595] = GetProcAddress(hL, "mpir_is_likely_prime_BPSW");
    p[596] = GetProcAddress(hL, "mpir_sqrt");
    p[597] = GetProcAddress(hL, "mpz_inp_raw_m");
    p[598] = GetProcAddress(hL, "mpz_inp_raw_p");
    p[599] = GetProcAddress(hL, "mpz_out_raw_m");
    if (reason == DLL_PROCESS_DETACH)
    {
        FreeLibrary(hL);
        return 1;
    }

    return 1;
}

extern "C"
{
    FARPROC PA = NULL;
    int RunASM();

    /*	void PROXY_??5@YAAEAV?$basic_istream@DU?$char_traits@D@std@@@std@@AEAV01@PEAU__mpf_struct@@@Z() {
    		PA = p[0];
    		RunASM();
    	}
    	void PROXY_??5@YAAEAV?$basic_istream@DU?$char_traits@D@std@@@std@@AEAV01@PEAU__mpq_struct@@@Z() {
    		PA = p[1];
    		RunASM();
    	}
    	void PROXY_??5@YAAEAV?$basic_istream@DU?$char_traits@D@std@@@std@@AEAV01@PEAU__mpz_struct@@@Z() {
    		PA = p[2];
    		RunASM();
    	}
    	void PROXY_??6@YAAEAV?$basic_ostream@DU?$char_traits@D@std@@@std@@AEAV01@PEBU__mpf_struct@@@Z() {
    		PA = p[3];
    		RunASM();
    	}
    	void PROXY_??6@YAAEAV?$basic_ostream@DU?$char_traits@D@std@@@std@@AEAV01@PEBU__mpq_struct@@@Z() {
    		PA = p[4];
    		RunASM();
    	}
    	void PROXY_??6@YAAEAV?$basic_ostream@DU?$char_traits@D@std@@@std@@AEAV01@PEBU__mpz_struct@@@Z() {
    		PA = p[5];
    		RunASM();
    	}*/
    void PROXY___combine_limbs() {
        PA = p[6];
        RunASM();
    }
    void PROXY___fermat_to_mpz() {
        PA = p[7];
        RunASM();
    }
    void PROXY___gmp_0() {
        PA = p[8];
        RunASM();
    }
    void PROXY___gmp_allocate_func() {
        PA = p[9];
        RunASM();
    }
    void PROXY___gmp_asprintf() {
        PA = p[10];
        RunASM();
    }
    void PROXY___gmp_asprintf_final() {
        PA = p[11];
        RunASM();
    }
    void PROXY___gmp_asprintf_memory() {
        PA = p[12];
        RunASM();
    }
    void PROXY___gmp_asprintf_reps() {
        PA = p[13];
        RunASM();
    }
    void PROXY___gmp_assert_fail() {
        PA = p[14];
        RunASM();
    }
    void PROXY___gmp_assert_header() {
        PA = p[15];
        RunASM();
    }
    void PROXY___gmp_bits_per_limb() {
        PA = p[16];
        RunASM();
    }
    void PROXY___gmp_default_allocate() {
        PA = p[17];
        RunASM();
    }
    void PROXY___gmp_default_fp_limb_precision() {
        PA = p[18];
        RunASM();
    }
    void PROXY___gmp_default_free() {
        PA = p[19];
        RunASM();
    }
    void PROXY___gmp_default_reallocate() {
        PA = p[20];
        RunASM();
    }
    void PROXY___gmp_digit_value_tab() {
        PA = p[21];
        RunASM();
    }
    void PROXY___gmp_divide_by_zero() {
        PA = p[22];
        RunASM();
    }
    void PROXY___gmp_doprnt() {
        PA = p[23];
        RunASM();
    }
    void PROXY___gmp_doprnt_integer() {
        PA = p[24];
        RunASM();
    }
    void PROXY___gmp_doprnt_mpf2() {
        PA = p[25];
        RunASM();
    }
    void PROXY___gmp_doscan() {
        PA = p[26];
        RunASM();
    }
    void PROXY___gmp_errno() {
        PA = p[27];
        RunASM();
    }
    void PROXY___gmp_exception() {
        PA = p[28];
        RunASM();
    }
    void PROXY___gmp_extract_double() {
        PA = p[29];
        RunASM();
    }
    void PROXY___gmp_fib_table() {
        PA = p[30];
        RunASM();
    }
    void PROXY___gmp_fprintf() {
        PA = p[31];
        RunASM();
    }
    void PROXY___gmp_free_func() {
        PA = p[32];
        RunASM();
    }
    void PROXY___gmp_fscanf() {
        PA = p[33];
        RunASM();
    }
    void PROXY___gmp_get_memory_functions() {
        PA = p[34];
        RunASM();
    }
    void PROXY___gmp_init_primesieve() {
        PA = p[35];
        RunASM();
    }
    void PROXY___gmp_invalid_operation() {
        PA = p[36];
        RunASM();
    }
    void PROXY___gmp_jacobi_table() {
        PA = p[37];
        RunASM();
    }
    void PROXY___gmp_junk() {
        PA = p[38];
        RunASM();
    }
    void PROXY___gmp_modlimb_invert_table() {
        PA = p[39];
        RunASM();
    }
    void PROXY___gmp_nextprime() {
        PA = p[40];
        RunASM();
    }
    void PROXY___gmp_primesieve() {
        PA = p[41];
        RunASM();
    }
    void PROXY___gmp_printf() {
        PA = p[42];
        RunASM();
    }
    void PROXY___gmp_randclear() {
        PA = p[43];
        RunASM();
    }
    void PROXY___gmp_randinit_default() {
        PA = p[44];
        RunASM();
    }
    void PROXY___gmp_randinit_lc_2exp() {
        PA = p[45];
        RunASM();
    }
    void PROXY___gmp_randinit_lc_2exp_size() {
        PA = p[46];
        RunASM();
    }
    void PROXY___gmp_randinit_mt() {
        PA = p[47];
        RunASM();
    }
    void PROXY___gmp_randinit_mt_noseed() {
        PA = p[48];
        RunASM();
    }
    void PROXY___gmp_randinit_set() {
        PA = p[49];
        RunASM();
    }
    void PROXY___gmp_rands() {
        PA = p[50];
        RunASM();
    }
    void PROXY___gmp_rands_initialized() {
        PA = p[51];
        RunASM();
    }
    void PROXY___gmp_randseed() {
        PA = p[52];
        RunASM();
    }
    void PROXY___gmp_randseed_ui() {
        PA = p[53];
        RunASM();
    }
    void PROXY___gmp_reallocate_func() {
        PA = p[54];
        RunASM();
    }
    void PROXY___gmp_replacement_vsnprintf() {
        PA = p[55];
        RunASM();
    }
    void PROXY___gmp_scanf() {
        PA = p[56];
        RunASM();
    }
    void PROXY___gmp_set_memory_functions() {
        PA = p[57];
        RunASM();
    }
    void PROXY___gmp_snprintf() {
        PA = p[58];
        RunASM();
    }
    void PROXY___gmp_sprintf() {
        PA = p[59];
        RunASM();
    }
    void PROXY___gmp_sqrt_of_negative() {
        PA = p[60];
        RunASM();
    }
    void PROXY___gmp_sscanf() {
        PA = p[61];
        RunASM();
    }
    void PROXY___gmp_tmp_reentrant_alloc() {
        PA = p[62];
        RunASM();
    }
    void PROXY___gmp_tmp_reentrant_free() {
        PA = p[63];
        RunASM();
    }
    void PROXY___gmp_urandomb_ui() {
        PA = p[64];
        RunASM();
    }
    void PROXY___gmp_urandomm_ui() {
        PA = p[65];
        RunASM();
    }
    void PROXY___gmp_vasprintf() {
        PA = p[66];
        RunASM();
    }
    void PROXY___gmp_version() {
        PA = p[67];
        RunASM();
    }
    void PROXY___gmp_vfprintf() {
        PA = p[68];
        RunASM();
    }
    void PROXY___gmp_vfscanf() {
        PA = p[69];
        RunASM();
    }
    void PROXY___gmp_vprintf() {
        PA = p[70];
        RunASM();
    }
    void PROXY___gmp_vscanf() {
        PA = p[71];
        RunASM();
    }
    void PROXY___gmp_vsnprintf() {
        PA = p[72];
        RunASM();
    }
    void PROXY___gmp_vsprintf() {
        PA = p[73];
        RunASM();
    }
    void PROXY___gmp_vsscanf() {
        PA = p[74];
        RunASM();
    }
    void PROXY___gmpf_abs() {
        PA = p[75];
        RunASM();
    }
    void PROXY___gmpf_add() {
        PA = p[76];
        RunASM();
    }
    void PROXY___gmpf_add_ui() {
        PA = p[77];
        RunASM();
    }
    void PROXY___gmpf_ceil() {
        PA = p[78];
        RunASM();
    }
    void PROXY___gmpf_clear() {
        PA = p[79];
        RunASM();
    }
    void PROXY___gmpf_clears() {
        PA = p[80];
        RunASM();
    }
    void PROXY___gmpf_cmp() {
        PA = p[81];
        RunASM();
    }
    void PROXY___gmpf_cmp_d() {
        PA = p[82];
        RunASM();
    }
    void PROXY___gmpf_cmp_si() {
        PA = p[83];
        RunASM();
    }
    void PROXY___gmpf_cmp_ui() {
        PA = p[84];
        RunASM();
    }
    void PROXY___gmpf_cmp_z() {
        PA = p[85];
        RunASM();
    }
    void PROXY___gmpf_div() {
        PA = p[86];
        RunASM();
    }
    void PROXY___gmpf_div_2exp() {
        PA = p[87];
        RunASM();
    }
    void PROXY___gmpf_div_ui() {
        PA = p[88];
        RunASM();
    }
    void PROXY___gmpf_dump() {
        PA = p[89];
        RunASM();
    }
    void PROXY___gmpf_eq() {
        PA = p[90];
        RunASM();
    }
    void PROXY___gmpf_fits_si_p() {
        PA = p[91];
        RunASM();
    }
    void PROXY___gmpf_fits_sint_p() {
        PA = p[92];
        RunASM();
    }
    void PROXY___gmpf_fits_slong_p() {
        PA = p[93];
        RunASM();
    }
    void PROXY___gmpf_fits_sshort_p() {
        PA = p[94];
        RunASM();
    }
    void PROXY___gmpf_fits_ui_p() {
        PA = p[95];
        RunASM();
    }
    void PROXY___gmpf_fits_uint_p() {
        PA = p[96];
        RunASM();
    }
    void PROXY___gmpf_fits_ulong_p() {
        PA = p[97];
        RunASM();
    }
    void PROXY___gmpf_fits_ushort_p() {
        PA = p[98];
        RunASM();
    }
    void PROXY___gmpf_floor() {
        PA = p[99];
        RunASM();
    }
    void PROXY___gmpf_get_2exp_d() {
        PA = p[100];
        RunASM();
    }
    void PROXY___gmpf_get_d() {
        PA = p[101];
        RunASM();
    }
    void PROXY___gmpf_get_d_2exp() {
        PA = p[102];
        RunASM();
    }
    void PROXY___gmpf_get_default_prec() {
        PA = p[103];
        RunASM();
    }
    void PROXY___gmpf_get_prec() {
        PA = p[104];
        RunASM();
    }
    void PROXY___gmpf_get_si() {
        PA = p[105];
        RunASM();
    }
    void PROXY___gmpf_get_str() {
        PA = p[106];
        RunASM();
    }
    void PROXY___gmpf_get_ui() {
        PA = p[107];
        RunASM();
    }
    void PROXY___gmpf_init() {
        PA = p[108];
        RunASM();
    }
    void PROXY___gmpf_init2() {
        PA = p[109];
        RunASM();
    }
    void PROXY___gmpf_init_set() {
        PA = p[110];
        RunASM();
    }
    void PROXY___gmpf_init_set_d() {
        PA = p[111];
        RunASM();
    }
    void PROXY___gmpf_init_set_si() {
        PA = p[112];
        RunASM();
    }
    void PROXY___gmpf_init_set_str() {
        PA = p[113];
        RunASM();
    }
    void PROXY___gmpf_init_set_ui() {
        PA = p[114];
        RunASM();
    }
    void PROXY___gmpf_inits() {
        PA = p[115];
        RunASM();
    }
    void PROXY___gmpf_inp_str() {
        PA = p[116];
        RunASM();
    }
    void PROXY___gmpf_integer_p() {
        PA = p[117];
        RunASM();
    }
    void PROXY___gmpf_mul() {
        PA = p[118];
        RunASM();
    }
    void PROXY___gmpf_mul_2exp() {
        PA = p[119];
        RunASM();
    }
    void PROXY___gmpf_mul_ui() {
        PA = p[120];
        RunASM();
    }
    void PROXY___gmpf_neg() {
        PA = p[121];
        RunASM();
    }
    void PROXY___gmpf_out_str() {
        PA = p[122];
        RunASM();
    }
    void PROXY___gmpf_pow_ui() {
        PA = p[123];
        RunASM();
    }
    void PROXY___gmpf_random2() {
        PA = p[124];
        RunASM();
    }
    void PROXY___gmpf_reldiff() {
        PA = p[125];
        RunASM();
    }
    void PROXY___gmpf_rrandomb() {
        PA = p[126];
        RunASM();
    }
    void PROXY___gmpf_set() {
        PA = p[127];
        RunASM();
    }
    void PROXY___gmpf_set_d() {
        PA = p[128];
        RunASM();
    }
    void PROXY___gmpf_set_default_prec() {
        PA = p[129];
        RunASM();
    }
    void PROXY___gmpf_set_prec() {
        PA = p[130];
        RunASM();
    }
    void PROXY___gmpf_set_prec_raw() {
        PA = p[131];
        RunASM();
    }
    void PROXY___gmpf_set_q() {
        PA = p[132];
        RunASM();
    }
    void PROXY___gmpf_set_si() {
        PA = p[133];
        RunASM();
    }
    void PROXY___gmpf_set_str() {
        PA = p[134];
        RunASM();
    }
    void PROXY___gmpf_set_ui() {
        PA = p[135];
        RunASM();
    }
    void PROXY___gmpf_set_z() {
        PA = p[136];
        RunASM();
    }
    void PROXY___gmpf_size() {
        PA = p[137];
        RunASM();
    }
    void PROXY___gmpf_sqrt() {
        PA = p[138];
        RunASM();
    }
    void PROXY___gmpf_sqrt_ui() {
        PA = p[139];
        RunASM();
    }
    void PROXY___gmpf_sub() {
        PA = p[140];
        RunASM();
    }
    void PROXY___gmpf_sub_ui() {
        PA = p[141];
        RunASM();
    }
    void PROXY___gmpf_swap() {
        PA = p[142];
        RunASM();
    }
    void PROXY___gmpf_trunc() {
        PA = p[143];
        RunASM();
    }
    void PROXY___gmpf_ui_div() {
        PA = p[144];
        RunASM();
    }
    void PROXY___gmpf_ui_sub() {
        PA = p[145];
        RunASM();
    }
    void PROXY___gmpf_urandomb() {
        PA = p[146];
        RunASM();
    }
    void PROXY___gmpn_add() {
        PA = p[147];
        RunASM();
    }
    void PROXY___gmpn_add_1() {
        PA = p[148];
        RunASM();
    }
    void PROXY___gmpn_add_err1_n() {
        PA = p[149];
        RunASM();
    }
    void PROXY___gmpn_add_err2_n() {
        PA = p[150];
        RunASM();
    }
    void PROXY___gmpn_add_n() {
        PA = p[151];
        RunASM();
    }
    void PROXY___gmpn_addadd_n() {
        PA = p[152];
        RunASM();
    }
    void PROXY___gmpn_addmul_1() {
        PA = p[153];
        RunASM();
    }
    void PROXY___gmpn_addmul_2() {
        PA = p[154];
        RunASM();
    }
    void PROXY___gmpn_addsub_n() {
        PA = p[155];
        RunASM();
    }
    void PROXY___gmpn_and_n() {
        PA = p[156];
        RunASM();
    }
    void PROXY___gmpn_andn_n() {
        PA = p[157];
        RunASM();
    }
    void PROXY___gmpn_bases() {
        PA = p[158];
        RunASM();
    }
    void PROXY___gmpn_bc_set_str() {
        PA = p[159];
        RunASM();
    }
    void PROXY___gmpn_bdivmod() {
        PA = p[160];
        RunASM();
    }
    void PROXY___gmpn_binvert() {
        PA = p[161];
        RunASM();
    }
    void PROXY___gmpn_binvert_itch() {
        PA = p[162];
        RunASM();
    }
    void PROXY___gmpn_clz_tab() {
        PA = p[163];
        RunASM();
    }
    void PROXY___gmpn_cmp() {
        PA = p[164];
        RunASM();
    }
    void PROXY___gmpn_com_n() {
        PA = p[165];
        RunASM();
    }
    void PROXY___gmpn_copyd() {
        PA = p[166];
        RunASM();
    }
    void PROXY___gmpn_copyi() {
        PA = p[167];
        RunASM();
    }
    void PROXY___gmpn_dc_bdiv_q() {
        PA = p[168];
        RunASM();
    }
    void PROXY___gmpn_dc_bdiv_q_n() {
        PA = p[169];
        RunASM();
    }
    void PROXY___gmpn_dc_bdiv_qr() {
        PA = p[170];
        RunASM();
    }
    void PROXY___gmpn_dc_bdiv_qr_n() {
        PA = p[171];
        RunASM();
    }
    void PROXY___gmpn_dc_div_q() {
        PA = p[172];
        RunASM();
    }
    void PROXY___gmpn_dc_div_qr() {
        PA = p[173];
        RunASM();
    }
    void PROXY___gmpn_dc_div_qr_n() {
        PA = p[174];
        RunASM();
    }
    void PROXY___gmpn_dc_divappr_q() {
        PA = p[175];
        RunASM();
    }
    void PROXY___gmpn_dc_set_str() {
        PA = p[176];
        RunASM();
    }
    void PROXY___gmpn_div_2expmod_2expp1() {
        PA = p[177];
        RunASM();
    }
    void PROXY___gmpn_divexact() {
        PA = p[178];
        RunASM();
    }
    void PROXY___gmpn_divexact_1() {
        PA = p[179];
        RunASM();
    }
    void PROXY___gmpn_divexact_by3c() {
        PA = p[180];
        RunASM();
    }
    void PROXY___gmpn_divexact_byff() {
        PA = p[181];
        RunASM();
    }
    void PROXY___gmpn_divexact_byfobm1() {
        PA = p[182];
        RunASM();
    }
    void PROXY___gmpn_divisible_p() {
        PA = p[183];
        RunASM();
    }
    void PROXY___gmpn_divrem() {
        PA = p[184];
        RunASM();
    }
    void PROXY___gmpn_divrem_1() {
        PA = p[185];
        RunASM();
    }
    void PROXY___gmpn_divrem_2() {
        PA = p[186];
        RunASM();
    }
    void PROXY___gmpn_divrem_euclidean_qr_1() {
        PA = p[187];
        RunASM();
    }
    void PROXY___gmpn_divrem_euclidean_qr_2() {
        PA = p[188];
        RunASM();
    }
    void PROXY___gmpn_divrem_euclidean_r_1() {
        PA = p[189];
        RunASM();
    }
    void PROXY___gmpn_divrem_hensel_qr_1() {
        PA = p[190];
        RunASM();
    }
    void PROXY___gmpn_divrem_hensel_qr_1_1() {
        PA = p[191];
        RunASM();
    }
    void PROXY___gmpn_divrem_hensel_qr_1_2() {
        PA = p[192];
        RunASM();
    }
    void PROXY___gmpn_divrem_hensel_r_1() {
        PA = p[193];
        RunASM();
    }
    void PROXY___gmpn_divrem_hensel_rsh_qr_1() {
        PA = p[194];
        RunASM();
    }
    void PROXY___gmpn_divrem_hensel_rsh_qr_1_preinv() {
        PA = p[195];
        RunASM();
    }
    void PROXY___gmpn_dump() {
        PA = p[196];
        RunASM();
    }
    void PROXY___gmpn_fib2_ui() {
        PA = p[197];
        RunASM();
    }
    void PROXY___gmpn_gcd() {
        PA = p[198];
        RunASM();
    }
    void PROXY___gmpn_gcd_1() {
        PA = p[199];
        RunASM();
    }
    void PROXY___gmpn_gcd_subdiv_step() {
        PA = p[200];
        RunASM();
    }
    void PROXY___gmpn_gcdext() {
        PA = p[201];
        RunASM();
    }
    void PROXY___gmpn_gcdext_1() {
        PA = p[202];
        RunASM();
    }
    void PROXY___gmpn_gcdext_hook() {
        PA = p[203];
        RunASM();
    }
    void PROXY___gmpn_gcdext_lehmer_n() {
        PA = p[204];
        RunASM();
    }
    void PROXY___gmpn_get_d() {
        PA = p[205];
        RunASM();
    }
    void PROXY___gmpn_get_str() {
        PA = p[206];
        RunASM();
    }
    void PROXY___gmpn_hamdist() {
        PA = p[207];
        RunASM();
    }
    void PROXY___gmpn_hgcd() {
        PA = p[208];
        RunASM();
    }
    void PROXY___gmpn_hgcd2() {
        PA = p[209];
        RunASM();
    }
    void PROXY___gmpn_hgcd2_jacobi() {
        PA = p[210];
        RunASM();
    }
    void PROXY___gmpn_hgcd_appr() {
        PA = p[211];
        RunASM();
    }
    void PROXY___gmpn_hgcd_appr_itch() {
        PA = p[212];
        RunASM();
    }
    void PROXY___gmpn_hgcd_itch() {
        PA = p[213];
        RunASM();
    }
    void PROXY___gmpn_hgcd_jacobi() {
        PA = p[214];
        RunASM();
    }
    void PROXY___gmpn_hgcd_matrix_adjust() {
        PA = p[215];
        RunASM();
    }
    void PROXY___gmpn_hgcd_matrix_init() {
        PA = p[216];
        RunASM();
    }
    void PROXY___gmpn_hgcd_matrix_mul() {
        PA = p[217];
        RunASM();
    }
    void PROXY___gmpn_hgcd_matrix_mul_1() {
        PA = p[218];
        RunASM();
    }
    void PROXY___gmpn_hgcd_matrix_update_q() {
        PA = p[219];
        RunASM();
    }
    void PROXY___gmpn_hgcd_mul_matrix1_vector() {
        PA = p[220];
        RunASM();
    }
    void PROXY___gmpn_hgcd_reduce() {
        PA = p[221];
        RunASM();
    }
    void PROXY___gmpn_hgcd_reduce_itch() {
        PA = p[222];
        RunASM();
    }
    void PROXY___gmpn_hgcd_step() {
        PA = p[223];
        RunASM();
    }
    void PROXY___gmpn_inv_div_q() {
        PA = p[224];
        RunASM();
    }
    void PROXY___gmpn_inv_div_qr() {
        PA = p[225];
        RunASM();
    }
    void PROXY___gmpn_inv_div_qr_n() {
        PA = p[226];
        RunASM();
    }
    void PROXY___gmpn_inv_divappr_q() {
        PA = p[227];
        RunASM();
    }
    void PROXY___gmpn_inv_divappr_q_n() {
        PA = p[228];
        RunASM();
    }
    void PROXY___gmpn_invert() {
        PA = p[229];
        RunASM();
    }
    void PROXY___gmpn_invert_trunc() {
        PA = p[230];
        RunASM();
    }
    void PROXY___gmpn_ior_n() {
        PA = p[231];
        RunASM();
    }
    void PROXY___gmpn_iorn_n() {
        PA = p[232];
        RunASM();
    }
    void PROXY___gmpn_is_invert() {
        PA = p[233];
        RunASM();
    }
    void PROXY___gmpn_jacobi_2() {
        PA = p[234];
        RunASM();
    }
    void PROXY___gmpn_jacobi_base() {
        PA = p[235];
        RunASM();
    }
    void PROXY___gmpn_jacobi_n() {
        PA = p[236];
        RunASM();
    }
    void PROXY___gmpn_kara_mul_n() {
        PA = p[237];
        RunASM();
    }
    void PROXY___gmpn_kara_sqr_n() {
        PA = p[238];
        RunASM();
    }
    void PROXY___gmpn_lshift() {
        PA = p[239];
        RunASM();
    }
    void PROXY___gmpn_matrix22_mul() {
        PA = p[240];
        RunASM();
    }
    void PROXY___gmpn_matrix22_mul1_inverse_vector() {
        PA = p[241];
        RunASM();
    }
    void PROXY___gmpn_matrix22_mul_itch() {
        PA = p[242];
        RunASM();
    }
    void PROXY___gmpn_matrix22_mul_strassen() {
        PA = p[243];
        RunASM();
    }
    void PROXY___gmpn_mod_1() {
        PA = p[244];
        RunASM();
    }
    void PROXY___gmpn_mod_1_1() {
        PA = p[245];
        RunASM();
    }
    void PROXY___gmpn_mod_1_2() {
        PA = p[246];
        RunASM();
    }
    void PROXY___gmpn_mod_1_3() {
        PA = p[247];
        RunASM();
    }
    void PROXY___gmpn_mod_1_k() {
        PA = p[248];
        RunASM();
    }
    void PROXY___gmpn_mod_34lsub1() {
        PA = p[249];
        RunASM();
    }
    void PROXY___gmpn_modexact_1c_odd() {
        PA = p[250];
        RunASM();
    }
    void PROXY___gmpn_mul() {
        PA = p[251];
        RunASM();
    }
    void PROXY___gmpn_mul_1() {
        PA = p[252];
        RunASM();
    }
    void PROXY___gmpn_mul_2expmod_2expp1() {
        PA = p[253];
        RunASM();
    }
    void PROXY___gmpn_mul_basecase() {
        PA = p[254];
        RunASM();
    }
    void PROXY___gmpn_mul_fft() {
        PA = p[255];
        RunASM();
    }
    void PROXY___gmpn_mul_fft_main() {
        PA = p[256];
        RunASM();
    }
    void PROXY___gmpn_mul_mfa_trunc_sqrt2() {
        PA = p[257];
        RunASM();
    }
    void PROXY___gmpn_mul_n() {
        PA = p[258];
        RunASM();
    }
    void PROXY___gmpn_mul_trunc_sqrt2() {
        PA = p[259];
        RunASM();
    }
    void PROXY___gmpn_mulhigh_n() {
        PA = p[260];
        RunASM();
    }
    void PROXY___gmpn_mullow_basecase() {
        PA = p[261];
        RunASM();
    }
    void PROXY___gmpn_mullow_n() {
        PA = p[262];
        RunASM();
    }
    void PROXY___gmpn_mullow_n_basecase() {
        PA = p[263];
        RunASM();
    }
    void PROXY___gmpn_mulmid() {
        PA = p[264];
        RunASM();
    }
    void PROXY___gmpn_mulmid_basecase() {
        PA = p[265];
        RunASM();
    }
    void PROXY___gmpn_mulmid_n() {
        PA = p[266];
        RunASM();
    }
    void PROXY___gmpn_mulmod_2expm1() {
        PA = p[267];
        RunASM();
    }
    void PROXY___gmpn_mulmod_2expp1_basecase() {
        PA = p[268];
        RunASM();
    }
    void PROXY___gmpn_mulmod_Bexpp1() {
        PA = p[269];
        RunASM();
    }
    void PROXY___gmpn_mulmod_Bexpp1_fft() {
        PA = p[270];
        RunASM();
    }
    void PROXY___gmpn_mulmod_bnm1() {
        PA = p[271];
        RunASM();
    }
    void PROXY___gmpn_nand_n() {
        PA = p[272];
        RunASM();
    }
    void PROXY___gmpn_nior_n() {
        PA = p[273];
        RunASM();
    }
    void PROXY___gmpn_normmod_2expp1() {
        PA = p[274];
        RunASM();
    }
    void PROXY___gmpn_nsumdiff_n() {
        PA = p[275];
        RunASM();
    }
    void PROXY___gmpn_perfect_square_p() {
        PA = p[276];
        RunASM();
    }
    void PROXY___gmpn_popcount() {
        PA = p[277];
        RunASM();
    }
    void PROXY___gmpn_pow_1() {
        PA = p[278];
        RunASM();
    }
    void PROXY___gmpn_powlo() {
        PA = p[279];
        RunASM();
    }
    void PROXY___gmpn_powm() {
        PA = p[280];
        RunASM();
    }
    void PROXY___gmpn_preinv_divrem_1() {
        PA = p[281];
        RunASM();
    }
    void PROXY___gmpn_preinv_mod_1() {
        PA = p[282];
        RunASM();
    }
    void PROXY___gmpn_random() {
        PA = p[283];
        RunASM();
    }
    void PROXY___gmpn_random2() {
        PA = p[284];
        RunASM();
    }
    void PROXY___gmpn_randomb() {
        PA = p[285];
        RunASM();
    }
    void PROXY___gmpn_redc_1() {
        PA = p[286];
        RunASM();
    }
    void PROXY___gmpn_redc_2() {
        PA = p[287];
        RunASM();
    }
    void PROXY___gmpn_redc_n() {
        PA = p[288];
        RunASM();
    }
    void PROXY___gmpn_rootrem() {
        PA = p[289];
        RunASM();
    }
    void PROXY___gmpn_rootrem_basecase() {
        PA = p[290];
        RunASM();
    }
    void PROXY___gmpn_rrandom() {
        PA = p[291];
        RunASM();
    }
    void PROXY___gmpn_rsh_divrem_hensel_qr_1() {
        PA = p[292];
        RunASM();
    }
    void PROXY___gmpn_rsh_divrem_hensel_qr_1_1() {
        PA = p[293];
        RunASM();
    }
    void PROXY___gmpn_rsh_divrem_hensel_qr_1_2() {
        PA = p[294];
        RunASM();
    }
    void PROXY___gmpn_rshift() {
        PA = p[295];
        RunASM();
    }
    void PROXY___gmpn_sb_bdiv_q() {
        PA = p[296];
        RunASM();
    }
    void PROXY___gmpn_sb_bdiv_qr() {
        PA = p[297];
        RunASM();
    }
    void PROXY___gmpn_sb_div_q() {
        PA = p[298];
        RunASM();
    }
    void PROXY___gmpn_sb_div_qr() {
        PA = p[299];
        RunASM();
    }
    void PROXY___gmpn_sb_divappr_q() {
        PA = p[300];
        RunASM();
    }
    void PROXY___gmpn_scan0() {
        PA = p[301];
        RunASM();
    }
    void PROXY___gmpn_scan1() {
        PA = p[302];
        RunASM();
    }
    void PROXY___gmpn_set_str() {
        PA = p[303];
        RunASM();
    }
    void PROXY___gmpn_set_str_compute_powtab() {
        PA = p[304];
        RunASM();
    }
    void PROXY___gmpn_sizeinbase() {
        PA = p[305];
        RunASM();
    }
    void PROXY___gmpn_sqr() {
        PA = p[306];
        RunASM();
    }
    void PROXY___gmpn_sqr_basecase() {
        PA = p[307];
        RunASM();
    }
    void PROXY___gmpn_sqrtrem() {
        PA = p[308];
        RunASM();
    }
    void PROXY___gmpn_sub() {
        PA = p[309];
        RunASM();
    }
    void PROXY___gmpn_sub_1() {
        PA = p[310];
        RunASM();
    }
    void PROXY___gmpn_sub_err1_n() {
        PA = p[311];
        RunASM();
    }
    void PROXY___gmpn_sub_err2_n() {
        PA = p[312];
        RunASM();
    }
    void PROXY___gmpn_sub_n() {
        PA = p[313];
        RunASM();
    }
    void PROXY___gmpn_subadd_n() {
        PA = p[314];
        RunASM();
    }
    void PROXY___gmpn_submul_1() {
        PA = p[315];
        RunASM();
    }
    void PROXY___gmpn_sumdiff_n() {
        PA = p[316];
        RunASM();
    }
    void PROXY___gmpn_tdiv_q() {
        PA = p[317];
        RunASM();
    }
    void PROXY___gmpn_tdiv_qr() {
        PA = p[318];
        RunASM();
    }
    void PROXY___gmpn_toom32_mul() {
        PA = p[319];
        RunASM();
    }
    void PROXY___gmpn_toom3_interpolate() {
        PA = p[320];
        RunASM();
    }
    void PROXY___gmpn_toom3_mul() {
        PA = p[321];
        RunASM();
    }
    void PROXY___gmpn_toom3_mul_n() {
        PA = p[322];
        RunASM();
    }
    void PROXY___gmpn_toom3_sqr_n() {
        PA = p[323];
        RunASM();
    }
    void PROXY___gmpn_toom42_mul() {
        PA = p[324];
        RunASM();
    }
    void PROXY___gmpn_toom42_mulmid() {
        PA = p[325];
        RunASM();
    }
    void PROXY___gmpn_toom4_interpolate() {
        PA = p[326];
        RunASM();
    }
    void PROXY___gmpn_toom4_mul() {
        PA = p[327];
        RunASM();
    }
    void PROXY___gmpn_toom4_mul_n() {
        PA = p[328];
        RunASM();
    }
    void PROXY___gmpn_toom4_sqr_n() {
        PA = p[329];
        RunASM();
    }
    void PROXY___gmpn_toom53_mul() {
        PA = p[330];
        RunASM();
    }
    void PROXY___gmpn_toom8_sqr_n() {
        PA = p[331];
        RunASM();
    }
    void PROXY___gmpn_toom8h_mul() {
        PA = p[332];
        RunASM();
    }
    void PROXY___gmpn_toom_couple_handling() {
        PA = p[333];
        RunASM();
    }
    void PROXY___gmpn_toom_eval_dgr3_pm1() {
        PA = p[334];
        RunASM();
    }
    void PROXY___gmpn_toom_eval_dgr3_pm2() {
        PA = p[335];
        RunASM();
    }
    void PROXY___gmpn_toom_eval_pm1() {
        PA = p[336];
        RunASM();
    }
    void PROXY___gmpn_toom_eval_pm2() {
        PA = p[337];
        RunASM();
    }
    void PROXY___gmpn_toom_eval_pm2exp() {
        PA = p[338];
        RunASM();
    }
    void PROXY___gmpn_toom_eval_pm2rexp() {
        PA = p[339];
        RunASM();
    }
    void PROXY___gmpn_toom_interpolate_16pts() {
        PA = p[340];
        RunASM();
    }
    void PROXY___gmpn_urandomb() {
        PA = p[341];
        RunASM();
    }
    void PROXY___gmpn_urandomm() {
        PA = p[342];
        RunASM();
    }
    void PROXY___gmpn_xnor_n() {
        PA = p[343];
        RunASM();
    }
    void PROXY___gmpn_xor_n() {
        PA = p[344];
        RunASM();
    }
    void PROXY___gmpn_zero() {
        PA = p[345];
        RunASM();
    }
    void PROXY___gmpn_zero_p() {
        PA = p[346];
        RunASM();
    }
    void PROXY___gmpq_abs() {
        PA = p[347];
        RunASM();
    }
    void PROXY___gmpq_add() {
        PA = p[348];
        RunASM();
    }
    void PROXY___gmpq_canonicalize() {
        PA = p[349];
        RunASM();
    }
    void PROXY___gmpq_clear() {
        PA = p[350];
        RunASM();
    }
    void PROXY___gmpq_clears() {
        PA = p[351];
        RunASM();
    }
    void PROXY___gmpq_cmp() {
        PA = p[352];
        RunASM();
    }
    void PROXY___gmpq_cmp_si() {
        PA = p[353];
        RunASM();
    }
    void PROXY___gmpq_cmp_ui() {
        PA = p[354];
        RunASM();
    }
    void PROXY___gmpq_cmp_z() {
        PA = p[355];
        RunASM();
    }
    void PROXY___gmpq_div() {
        PA = p[356];
        RunASM();
    }
    void PROXY___gmpq_div_2exp() {
        PA = p[357];
        RunASM();
    }
    void PROXY___gmpq_equal() {
        PA = p[358];
        RunASM();
    }
    void PROXY___gmpq_get_d() {
        PA = p[359];
        RunASM();
    }
    void PROXY___gmpq_get_den() {
        PA = p[360];
        RunASM();
    }
    void PROXY___gmpq_get_num() {
        PA = p[361];
        RunASM();
    }
    void PROXY___gmpq_get_str() {
        PA = p[362];
        RunASM();
    }
    void PROXY___gmpq_init() {
        PA = p[363];
        RunASM();
    }
    void PROXY___gmpq_inits() {
        PA = p[364];
        RunASM();
    }
    void PROXY___gmpq_inp_str() {
        PA = p[365];
        RunASM();
    }
    void PROXY___gmpq_inv() {
        PA = p[366];
        RunASM();
    }
    void PROXY___gmpq_mul() {
        PA = p[367];
        RunASM();
    }
    void PROXY___gmpq_mul_2exp() {
        PA = p[368];
        RunASM();
    }
    void PROXY___gmpq_neg() {
        PA = p[369];
        RunASM();
    }
    void PROXY___gmpq_out_str() {
        PA = p[370];
        RunASM();
    }
    void PROXY___gmpq_set() {
        PA = p[371];
        RunASM();
    }
    void PROXY___gmpq_set_d() {
        PA = p[372];
        RunASM();
    }
    void PROXY___gmpq_set_den() {
        PA = p[373];
        RunASM();
    }
    void PROXY___gmpq_set_f() {
        PA = p[374];
        RunASM();
    }
    void PROXY___gmpq_set_num() {
        PA = p[375];
        RunASM();
    }
    void PROXY___gmpq_set_si() {
        PA = p[376];
        RunASM();
    }
    void PROXY___gmpq_set_str() {
        PA = p[377];
        RunASM();
    }
    void PROXY___gmpq_set_ui() {
        PA = p[378];
        RunASM();
    }
    void PROXY___gmpq_set_z() {
        PA = p[379];
        RunASM();
    }
    void PROXY___gmpq_sub() {
        PA = p[380];
        RunASM();
    }
    void PROXY___gmpq_swap() {
        PA = p[381];
        RunASM();
    }
    void PROXY___gmpz_2fac_ui() {
        PA = p[382];
        RunASM();
    }
    void PROXY___gmpz_abs() {
        PA = p[383];
        RunASM();
    }
    void PROXY___gmpz_add() {
        PA = p[384];
        RunASM();
    }
    void PROXY___gmpz_add_ui() {
        PA = p[385];
        RunASM();
    }
    void PROXY___gmpz_addmul() {
        PA = p[386];
        RunASM();
    }
    void PROXY___gmpz_addmul_ui() {
        PA = p[387];
        RunASM();
    }
    void PROXY___gmpz_and() {
        PA = p[388];
        RunASM();
    }
    void PROXY___gmpz_aorsmul_1() {
        PA = p[389];
        RunASM();
    }
    void PROXY___gmpz_array_init() {
        PA = p[390];
        RunASM();
    }
    void PROXY___gmpz_bin_ui() {
        PA = p[391];
        RunASM();
    }
    void PROXY___gmpz_bin_uiui() {
        PA = p[392];
        RunASM();
    }
    void PROXY___gmpz_cdiv_q() {
        PA = p[393];
        RunASM();
    }
    void PROXY___gmpz_cdiv_q_2exp() {
        PA = p[394];
        RunASM();
    }
    void PROXY___gmpz_cdiv_q_ui() {
        PA = p[395];
        RunASM();
    }
    void PROXY___gmpz_cdiv_qr() {
        PA = p[396];
        RunASM();
    }
    void PROXY___gmpz_cdiv_qr_ui() {
        PA = p[397];
        RunASM();
    }
    void PROXY___gmpz_cdiv_r() {
        PA = p[398];
        RunASM();
    }
    void PROXY___gmpz_cdiv_r_2exp() {
        PA = p[399];
        RunASM();
    }
    void PROXY___gmpz_cdiv_r_ui() {
        PA = p[400];
        RunASM();
    }
    void PROXY___gmpz_cdiv_ui() {
        PA = p[401];
        RunASM();
    }
    void PROXY___gmpz_clear() {
        PA = p[402];
        RunASM();
    }
    void PROXY___gmpz_clears() {
        PA = p[403];
        RunASM();
    }
    void PROXY___gmpz_clrbit() {
        PA = p[404];
        RunASM();
    }
    void PROXY___gmpz_cmp() {
        PA = p[405];
        RunASM();
    }
    void PROXY___gmpz_cmp_d() {
        PA = p[406];
        RunASM();
    }
    void PROXY___gmpz_cmp_si() {
        PA = p[407];
        RunASM();
    }
    void PROXY___gmpz_cmp_ui() {
        PA = p[408];
        RunASM();
    }
    void PROXY___gmpz_cmpabs() {
        PA = p[409];
        RunASM();
    }
    void PROXY___gmpz_cmpabs_d() {
        PA = p[410];
        RunASM();
    }
    void PROXY___gmpz_cmpabs_ui() {
        PA = p[411];
        RunASM();
    }
    void PROXY___gmpz_com() {
        PA = p[412];
        RunASM();
    }
    void PROXY___gmpz_combit() {
        PA = p[413];
        RunASM();
    }
    void PROXY___gmpz_congruent_2exp_p() {
        PA = p[414];
        RunASM();
    }
    void PROXY___gmpz_congruent_p() {
        PA = p[415];
        RunASM();
    }
    void PROXY___gmpz_congruent_ui_p() {
        PA = p[416];
        RunASM();
    }
    void PROXY___gmpz_divexact() {
        PA = p[417];
        RunASM();
    }
    void PROXY___gmpz_divexact_gcd() {
        PA = p[418];
        RunASM();
    }
    void PROXY___gmpz_divexact_ui() {
        PA = p[419];
        RunASM();
    }
    void PROXY___gmpz_divisible_2exp_p() {
        PA = p[420];
        RunASM();
    }
    void PROXY___gmpz_divisible_p() {
        PA = p[421];
        RunASM();
    }
    void PROXY___gmpz_divisible_ui_p() {
        PA = p[422];
        RunASM();
    }
    void PROXY___gmpz_dump() {
        PA = p[423];
        RunASM();
    }
    void PROXY___gmpz_export() {
        PA = p[424];
        RunASM();
    }
    void PROXY___gmpz_fac_ui() {
        PA = p[425];
        RunASM();
    }
    void PROXY___gmpz_fdiv_q() {
        PA = p[426];
        RunASM();
    }
    void PROXY___gmpz_fdiv_q_2exp() {
        PA = p[427];
        RunASM();
    }
    void PROXY___gmpz_fdiv_q_ui() {
        PA = p[428];
        RunASM();
    }
    void PROXY___gmpz_fdiv_qr() {
        PA = p[429];
        RunASM();
    }
    void PROXY___gmpz_fdiv_qr_ui() {
        PA = p[430];
        RunASM();
    }
    void PROXY___gmpz_fdiv_r() {
        PA = p[431];
        RunASM();
    }
    void PROXY___gmpz_fdiv_r_2exp() {
        PA = p[432];
        RunASM();
    }
    void PROXY___gmpz_fdiv_r_ui() {
        PA = p[433];
        RunASM();
    }
    void PROXY___gmpz_fdiv_ui() {
        PA = p[434];
        RunASM();
    }
    void PROXY___gmpz_fib2_ui() {
        PA = p[435];
        RunASM();
    }
    void PROXY___gmpz_fib_ui() {
        PA = p[436];
        RunASM();
    }
    void PROXY___gmpz_fits_si_p() {
        PA = p[437];
        RunASM();
    }
    void PROXY___gmpz_fits_sint_p() {
        PA = p[438];
        RunASM();
    }
    void PROXY___gmpz_fits_slong_p() {
        PA = p[439];
        RunASM();
    }
    void PROXY___gmpz_fits_sshort_p() {
        PA = p[440];
        RunASM();
    }
    void PROXY___gmpz_fits_ui_p() {
        PA = p[441];
        RunASM();
    }
    void PROXY___gmpz_fits_uint_p() {
        PA = p[442];
        RunASM();
    }
    void PROXY___gmpz_fits_ulong_p() {
        PA = p[443];
        RunASM();
    }
    void PROXY___gmpz_fits_ushort_p() {
        PA = p[444];
        RunASM();
    }
    void PROXY___gmpz_gcd() {
        PA = p[445];
        RunASM();
    }
    void PROXY___gmpz_gcd_ui() {
        PA = p[446];
        RunASM();
    }
    void PROXY___gmpz_gcdext() {
        PA = p[447];
        RunASM();
    }
    void PROXY___gmpz_get_2exp_d() {
        PA = p[448];
        RunASM();
    }
    void PROXY___gmpz_get_d() {
        PA = p[449];
        RunASM();
    }
    void PROXY___gmpz_get_d_2exp() {
        PA = p[450];
        RunASM();
    }
    void PROXY___gmpz_get_si() {
        PA = p[451];
        RunASM();
    }
    void PROXY___gmpz_get_str() {
        PA = p[452];
        RunASM();
    }
    void PROXY___gmpz_get_sx() {
        PA = p[453];
        RunASM();
    }
    void PROXY___gmpz_get_ui() {
        PA = p[454];
        RunASM();
    }
    void PROXY___gmpz_get_ux() {
        PA = p[455];
        RunASM();
    }
    void PROXY___gmpz_getlimbn() {
        PA = p[456];
        RunASM();
    }
    void PROXY___gmpz_hamdist() {
        PA = p[457];
        RunASM();
    }
    void PROXY___gmpz_import() {
        PA = p[458];
        RunASM();
    }
    void PROXY___gmpz_init() {
        PA = p[459];
        RunASM();
    }
    void PROXY___gmpz_init2() {
        PA = p[460];
        RunASM();
    }
    void PROXY___gmpz_init_set() {
        PA = p[461];
        RunASM();
    }
    void PROXY___gmpz_init_set_d() {
        PA = p[462];
        RunASM();
    }
    void PROXY___gmpz_init_set_si() {
        PA = p[463];
        RunASM();
    }
    void PROXY___gmpz_init_set_str() {
        PA = p[464];
        RunASM();
    }
    void PROXY___gmpz_init_set_sx() {
        PA = p[465];
        RunASM();
    }
    void PROXY___gmpz_init_set_ui() {
        PA = p[466];
        RunASM();
    }
    void PROXY___gmpz_init_set_ux() {
        PA = p[467];
        RunASM();
    }
    void PROXY___gmpz_inits() {
        PA = p[468];
        RunASM();
    }
    void PROXY___gmpz_inp_raw() {
        PA = p[469];
        RunASM();
    }
    void PROXY___gmpz_inp_str() {
        PA = p[470];
        RunASM();
    }
    void PROXY___gmpz_inp_str_nowhite() {
        PA = p[471];
        RunASM();
    }
    void PROXY___gmpz_invert() {
        PA = p[472];
        RunASM();
    }
    void PROXY___gmpz_ior() {
        PA = p[473];
        RunASM();
    }
    void PROXY___gmpz_jacobi() {
        PA = p[474];
        RunASM();
    }
    void PROXY___gmpz_kronecker_si() {
        PA = p[475];
        RunASM();
    }
    void PROXY___gmpz_kronecker_ui() {
        PA = p[476];
        RunASM();
    }
    void PROXY___gmpz_lcm() {
        PA = p[477];
        RunASM();
    }
    void PROXY___gmpz_lcm_ui() {
        PA = p[478];
        RunASM();
    }
    void PROXY___gmpz_likely_prime_p() {
        PA = p[479];
        RunASM();
    }
    void PROXY___gmpz_limbs_finish() {
        PA = p[480];
        RunASM();
    }
    void PROXY___gmpz_limbs_modify() {
        PA = p[481];
        RunASM();
    }
    void PROXY___gmpz_limbs_read() {
        PA = p[482];
        RunASM();
    }
    void PROXY___gmpz_limbs_write() {
        PA = p[483];
        RunASM();
    }
    void PROXY___gmpz_lucnum2_ui() {
        PA = p[484];
        RunASM();
    }
    void PROXY___gmpz_lucnum_ui() {
        PA = p[485];
        RunASM();
    }
    void PROXY___gmpz_mfac_uiui() {
        PA = p[486];
        RunASM();
    }
    void PROXY___gmpz_miller_rabin() {
        PA = p[487];
        RunASM();
    }
    void PROXY___gmpz_millerrabin() {
        PA = p[488];
        RunASM();
    }
    void PROXY___gmpz_mod() {
        PA = p[489];
        RunASM();
    }
    void PROXY___gmpz_mul() {
        PA = p[490];
        RunASM();
    }
    void PROXY___gmpz_mul_2exp() {
        PA = p[491];
        RunASM();
    }
    void PROXY___gmpz_mul_si() {
        PA = p[492];
        RunASM();
    }
    void PROXY___gmpz_mul_ui() {
        PA = p[493];
        RunASM();
    }
    void PROXY___gmpz_n_pow_ui() {
        PA = p[494];
        RunASM();
    }
    void PROXY___gmpz_neg() {
        PA = p[495];
        RunASM();
    }
    void PROXY___gmpz_next_prime_candidate() {
        PA = p[496];
        RunASM();
    }
    void PROXY___gmpz_nextprime() {
        PA = p[497];
        RunASM();
    }
    void PROXY___gmpz_nthroot() {
        PA = p[498];
        RunASM();
    }
    void PROXY___gmpz_oddfac_1() {
        PA = p[499];
        RunASM();
    }
    void PROXY___gmpz_out_raw() {
        PA = p[500];
        RunASM();
    }
    void PROXY___gmpz_out_str() {
        PA = p[501];
        RunASM();
    }
    void PROXY___gmpz_perfect_power_p() {
        PA = p[502];
        RunASM();
    }
    void PROXY___gmpz_perfect_square_p() {
        PA = p[503];
        RunASM();
    }
    void PROXY___gmpz_popcount() {
        PA = p[504];
        RunASM();
    }
    void PROXY___gmpz_pow_ui() {
        PA = p[505];
        RunASM();
    }
    void PROXY___gmpz_powm() {
        PA = p[506];
        RunASM();
    }
    void PROXY___gmpz_powm_ui() {
        PA = p[507];
        RunASM();
    }
    void PROXY___gmpz_primorial_ui() {
        PA = p[508];
        RunASM();
    }
    void PROXY___gmpz_probab_prime_p() {
        PA = p[509];
        RunASM();
    }
    void PROXY___gmpz_probable_prime_p() {
        PA = p[510];
        RunASM();
    }
    void PROXY___gmpz_prodlimbs() {
        PA = p[511];
        RunASM();
    }
    void PROXY___gmpz_realloc() {
        PA = p[512];
        RunASM();
    }
    void PROXY___gmpz_realloc2() {
        PA = p[513];
        RunASM();
    }
    void PROXY___gmpz_remove() {
        PA = p[514];
        RunASM();
    }
    void PROXY___gmpz_roinit_n() {
        PA = p[515];
        RunASM();
    }
    void PROXY___gmpz_root() {
        PA = p[516];
        RunASM();
    }
    void PROXY___gmpz_rootrem() {
        PA = p[517];
        RunASM();
    }
    void PROXY___gmpz_rrandomb() {
        PA = p[518];
        RunASM();
    }
    void PROXY___gmpz_scan0() {
        PA = p[519];
        RunASM();
    }
    void PROXY___gmpz_scan1() {
        PA = p[520];
        RunASM();
    }
    void PROXY___gmpz_set() {
        PA = p[521];
        RunASM();
    }
    void PROXY___gmpz_set_d() {
        PA = p[522];
        RunASM();
    }
    void PROXY___gmpz_set_f() {
        PA = p[523];
        RunASM();
    }
    void PROXY___gmpz_set_q() {
        PA = p[524];
        RunASM();
    }
    void PROXY___gmpz_set_si() {
        PA = p[525];
        RunASM();
    }
    void PROXY___gmpz_set_str() {
        PA = p[526];
        RunASM();
    }
    void PROXY___gmpz_set_sx() {
        PA = p[527];
        RunASM();
    }
    void PROXY___gmpz_set_ui() {
        PA = p[528];
        RunASM();
    }
    void PROXY___gmpz_set_ux() {
        PA = p[529];
        RunASM();
    }
    void PROXY___gmpz_setbit() {
        PA = p[530];
        RunASM();
    }
    void PROXY___gmpz_si_kronecker() {
        PA = p[531];
        RunASM();
    }
    void PROXY___gmpz_size() {
        PA = p[532];
        RunASM();
    }
    void PROXY___gmpz_sizeinbase() {
        PA = p[533];
        RunASM();
    }
    void PROXY___gmpz_sqrt() {
        PA = p[534];
        RunASM();
    }
    void PROXY___gmpz_sqrtrem() {
        PA = p[535];
        RunASM();
    }
    void PROXY___gmpz_sub() {
        PA = p[536];
        RunASM();
    }
    void PROXY___gmpz_sub_ui() {
        PA = p[537];
        RunASM();
    }
    void PROXY___gmpz_submul() {
        PA = p[538];
        RunASM();
    }
    void PROXY___gmpz_submul_ui() {
        PA = p[539];
        RunASM();
    }
    void PROXY___gmpz_swap() {
        PA = p[540];
        RunASM();
    }
    void PROXY___gmpz_tdiv_q() {
        PA = p[541];
        RunASM();
    }
    void PROXY___gmpz_tdiv_q_2exp() {
        PA = p[542];
        RunASM();
    }
    void PROXY___gmpz_tdiv_q_ui() {
        PA = p[543];
        RunASM();
    }
    void PROXY___gmpz_tdiv_qr() {
        PA = p[544];
        RunASM();
    }
    void PROXY___gmpz_tdiv_qr_ui() {
        PA = p[545];
        RunASM();
    }
    void PROXY___gmpz_tdiv_r() {
        PA = p[546];
        RunASM();
    }
    void PROXY___gmpz_tdiv_r_2exp() {
        PA = p[547];
        RunASM();
    }
    void PROXY___gmpz_tdiv_r_ui() {
        PA = p[548];
        RunASM();
    }
    void PROXY___gmpz_tdiv_ui() {
        PA = p[549];
        RunASM();
    }
    void PROXY___gmpz_trial_division() {
        PA = p[550];
        RunASM();
    }
    void PROXY___gmpz_tstbit() {
        PA = p[551];
        RunASM();
    }
    void PROXY___gmpz_ui_kronecker() {
        PA = p[552];
        RunASM();
    }
    void PROXY___gmpz_ui_pow_ui() {
        PA = p[553];
        RunASM();
    }
    void PROXY___gmpz_ui_sub() {
        PA = p[554];
        RunASM();
    }
    void PROXY___gmpz_urandomb() {
        PA = p[555];
        RunASM();
    }
    void PROXY___gmpz_urandomm() {
        PA = p[556];
        RunASM();
    }
    void PROXY___gmpz_xor() {
        PA = p[557];
        RunASM();
    }
    void PROXY___mpir_butterfly_lshB() {
        PA = p[558];
        RunASM();
    }
    void PROXY___mpir_butterfly_rshB() {
        PA = p[559];
        RunASM();
    }
    void PROXY___mpir_fft_adjust() {
        PA = p[560];
        RunASM();
    }
    void PROXY___mpir_fft_adjust_limbs() {
        PA = p[561];
        RunASM();
    }
    void PROXY___mpir_fft_adjust_sqrt2() {
        PA = p[562];
        RunASM();
    }
    void PROXY___mpir_fft_butterfly() {
        PA = p[563];
        RunASM();
    }
    void PROXY___mpir_fft_butterfly_sqrt2() {
        PA = p[564];
        RunASM();
    }
    void PROXY___mpir_fft_butterfly_twiddle() {
        PA = p[565];
        RunASM();
    }
    void PROXY___mpir_fft_combine_bits() {
        PA = p[566];
        RunASM();
    }
    void PROXY___mpir_fft_mfa_trunc_sqrt2() {
        PA = p[567];
        RunASM();
    }
    void PROXY___mpir_fft_mfa_trunc_sqrt2_inner() {
        PA = p[568];
        RunASM();
    }
    void PROXY___mpir_fft_mfa_trunc_sqrt2_outer() {
        PA = p[569];
        RunASM();
    }
    void PROXY___mpir_fft_mulmod_2expp1() {
        PA = p[570];
        RunASM();
    }
    void PROXY___mpir_fft_naive_convolution_1() {
        PA = p[571];
        RunASM();
    }
    void PROXY___mpir_fft_negacyclic() {
        PA = p[572];
        RunASM();
    }
    void PROXY___mpir_fft_radix2() {
        PA = p[573];
        RunASM();
    }
    void PROXY___mpir_fft_radix2_twiddle() {
        PA = p[574];
        RunASM();
    }
    void PROXY___mpir_fft_split_bits() {
        PA = p[575];
        RunASM();
    }
    void PROXY___mpir_fft_split_limbs() {
        PA = p[576];
        RunASM();
    }
    void PROXY___mpir_fft_trunc() {
        PA = p[577];
        RunASM();
    }
    void PROXY___mpir_fft_trunc1() {
        PA = p[578];
        RunASM();
    }
    void PROXY___mpir_fft_trunc1_twiddle() {
        PA = p[579];
        RunASM();
    }
    void PROXY___mpir_fft_trunc_sqrt2() {
        PA = p[580];
        RunASM();
    }
    void PROXY___mpir_ifft_butterfly() {
        PA = p[581];
        RunASM();
    }
    void PROXY___mpir_ifft_butterfly_sqrt2() {
        PA = p[582];
        RunASM();
    }
    void PROXY___mpir_ifft_butterfly_twiddle() {
        PA = p[583];
        RunASM();
    }
    void PROXY___mpir_ifft_mfa_trunc_sqrt2() {
        PA = p[584];
        RunASM();
    }
    void PROXY___mpir_ifft_mfa_trunc_sqrt2_outer() {
        PA = p[585];
        RunASM();
    }
    void PROXY___mpir_ifft_negacyclic() {
        PA = p[586];
        RunASM();
    }
    void PROXY___mpir_ifft_radix2() {
        PA = p[587];
        RunASM();
    }
    void PROXY___mpir_ifft_radix2_twiddle() {
        PA = p[588];
        RunASM();
    }
    void PROXY___mpir_ifft_trunc() {
        PA = p[589];
        RunASM();
    }
    void PROXY___mpir_ifft_trunc1() {
        PA = p[590];
        RunASM();
    }
    void PROXY___mpir_ifft_trunc1_twiddle() {
        PA = p[591];
        RunASM();
    }
    void PROXY___mpir_ifft_trunc_sqrt2() {
        PA = p[592];
        RunASM();
    }
    void PROXY___mpir_revbin() {
        PA = p[593];
        RunASM();
    }
    void PROXY___mpir_version() {
        PA = p[594];
        RunASM();
    }
    void PROXY_mpir_is_likely_prime_BPSW() {
        PA = p[595];
        RunASM();
    }
    void PROXY_mpir_sqrt() {
        PA = p[596];
        RunASM();
    }
    void PROXY_mpz_inp_raw_m() {
        PA = p[597];
        RunASM();
    }
    void PROXY_mpz_inp_raw_p() {
        PA = p[598];
        RunASM();
    }
    void PROXY_mpz_out_raw_m() {
        PA = p[599];
        RunASM();
    }
}
