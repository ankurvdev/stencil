#pragma once
#include "enums.h"
#include "protocol_binary.h"
#include "protocol_cli.h"
#include "protocol_json.h"
#include "protocol_string.h"

#include "mutatorsaccessors.h"
#include "optionalprops.h"
#include "shared_tree.h"
#include "transactions.h"
#include "visitor.h"

#include "Database2.h"

#if defined USE_CPP_HTTPLIB
#include "WebService.h"
#endif
