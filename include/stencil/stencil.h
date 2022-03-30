#pragma once
#include "enums.h"
#include "mutatorsaccessors.h"
#include "optionalprops.h"
#include "shared_tree.h"
#include "timestamped.h"
#include "transactions.h"
#include "typetraits.h"
#include "typetraits_shared_string.h"
#include "uuidobject.h"
#include "visitor.h"

#include "Database2.h"
#include "cliserdes.h"
#include "protocol_binary.h"
#include "protocol_cli.h"
#include "protocol_json.h"
#include "protocol_string.h"

#if defined USE_CPP_HTTPLIB
#include "WebService.h"
#endif
