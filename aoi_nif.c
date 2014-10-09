#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "aoi.h"
#include "erl_nif.h"

struct aoi_space_nif{
    struct aoi_space* space;
    ErlNifEnv* send_env;
    ErlNifPid* self_pid;
};

struct aoi_send_ud{
    struct aoi_space_nif* space_nif;
    ErlNifEnv *env;
};

static ErlNifResourceType *AoiSpaceResourceType; 

static void
AoiSpaceResourceDtor(ErlNifEnv *env, void *obj){
    struct aoi_space_nif *AoiSpaceNif = obj;
    if(AoiSpaceNif->space)
        aoi_release(AoiSpaceNif->space);
    if(AoiSpaceNif->send_env)
        enif_free_env(AoiSpaceNif->send_env);
}

static void *
aoi_alloc(void *ud, void *ptr, size_t sz){
	if (ptr == NULL) {
		void *p = enif_alloc(sz);
		return p;
	}
	enif_free(ptr);
	return NULL;
}

static void 
aoi_callback(void *ud, uint32_t watcher, uint32_t marker){
    struct aoi_send_ud *AoiSendUd = ud;
    struct aoi_space_nif *AoiSpaceNif = AoiSendUd->space_nif;
    enif_send(AoiSendUd->env, AoiSpaceNif->self_pid, AoiSpaceNif->send_env, 
            enif_make_tuple3(
                AoiSpaceNif->send_env, 
                enif_make_atom(AoiSpaceNif->send_env, "aoi_msg"), 
                enif_make_uint(AoiSpaceNif->send_env, watcher),
                enif_make_uint(AoiSpaceNif->send_env, marker)
                )
            );
    enif_clear_env(AoiSpaceNif->send_env);
}

static int
aoi_load(ErlNifEnv *env, void **priv_data, ERL_NIF_TERM load_info){
    ErlNifResourceFlags tried;
    AoiSpaceResourceType = enif_open_resource_type(env, NULL, "aoi_space", AoiSpaceResourceDtor, ERL_NIF_RT_CREATE,&tried);
	return 0;
};


static void 
aoi_unload(ErlNifEnv *env, void *priv_data){
}

static ERL_NIF_TERM 
aoi_create_nif(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]){
    ErlNifPid* self_pid;
    enif_self(env, self_pid);

    struct aoi_space_nif *AoiSpaceNif = enif_alloc_resource(AoiSpaceResourceType,sizeof(struct aoi_space_nif));
    AoiSpaceNif->space = aoi_create(aoi_alloc, NULL);
    AoiSpaceNif->send_env = enif_alloc_env();
    AoiSpaceNif->self_pid = self_pid;

    ERL_NIF_TERM result = enif_make_resource(env, AoiSpaceNif);
    enif_release_resource(AoiSpaceNif);
    return result;
}

static ERL_NIF_TERM 
aoi_update_nif(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]){
    struct aoi_space_nif *AoiSpaceNif;
    if(!enif_get_resource(env, argv[0], AoiSpaceResourceType, (void**)&AoiSpaceNif)){
         return enif_make_atom(env, "in aoi_update_nif, arg[0] is not a handle to a resource object");
    }

    unsigned int id;
    enif_get_uint(env, argv[1], &id);

    size_t mode_size = 4; // max "wmd" + 1
    char mode[mode_size];
    if( enif_get_atom(env, argv[2], mode, mode_size, ERL_NIF_LATIN1) <= 0){/*获取失败*/
        strcpy(mode, "");/**读取默认值*/
    }

    double pos[3];
    enif_get_double(env, argv[3], &pos[0]);
    enif_get_double(env, argv[4], &pos[1]);
    enif_get_double(env, argv[5], &pos[2]);

    float pos1[3] = {pos[0], pos[1], pos[2]};

    aoi_update(AoiSpaceNif->space, id, mode, pos1);

	return enif_make_atom(env, "ok"); 
}

static ERL_NIF_TERM 
aoi_message_nif(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]){
    struct aoi_space_nif *AoiSpaceNif;
    if(!enif_get_resource(env, argv[0], AoiSpaceResourceType, (void**)&AoiSpaceNif)){
         return enif_make_atom(env, "in aoi_message_nif, arg[0] is not a handle to a resource object");
    }
    struct aoi_send_ud AoiSendUd = {AoiSpaceNif, env};
    aoi_message(AoiSpaceNif->space, aoi_callback, &AoiSendUd);

	return enif_make_atom(env, "ok"); 
}



static ErlNifFunc nif_funcs[] = {
	{"create", 0, aoi_create_nif},
	{"update", 6, aoi_update_nif},
    {"message", 1, aoi_message_nif},
};

ERL_NIF_INIT(aoi_nif, nif_funcs, aoi_load, NULL, NULL, aoi_unload);

