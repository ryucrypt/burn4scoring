#include <burn4scoring.hpp>

/**
 * Upserts a score configuration
 * 
 * @param {uint32_t} range - upper limit of issued supply (inclusive)
 * @param {uint32_t} score - corresponding score
 * @auth - the contract itself
 */
ACTION burn4scoring::upsertconf(
    uint32_t range,
    uint32_t score
) {
    // Check for contract auth
    require_auth(get_self());

    // Get config table
    config_t configs(get_self(), get_self().value);
    auto iterator = configs.find(range);

    // Modify if exist, otherwise add new row
    if (iterator == configs.end()) {
        configs.emplace(get_self(), [&](auto &row) {
            row.range = range;
            row.score = score;
        });
    } else {
        configs.modify(iterator, get_self(), [&](auto &row) {
            row.score = score;
        });
    }
}

/**
 * Removes a score configuration
 * 
 * @param {uint32_t} range - upper limit of issued supply (inclusive)
 * @auth - the contract itself
 */
ACTION burn4scoring::remconf(
    uint32_t range
) {
     // Check for contract auth
    require_auth(get_self());

    // Get config table
    config_t configs(get_self(), get_self().value);
    auto iterator = configs.find(range);

    // Remove if exist
    check(iterator != configs.end(), "Record does not exist");
    configs.erase(iterator);
}

/**
 * Adds a collection to track
 * 
 * @param {name} collection - collection to track
 * @auth - the contract itself
 */
ACTION burn4scoring::addallow(
    name collection
) {
    // Check for contract auth
    require_auth(get_self());

    // Get allowed table
    allowed_t allowed(get_self(), get_self().value);
    auto iterator = allowed.find(collection.value);

    // Add if not exist
    if (iterator == allowed.end()) {
        allowed.emplace(get_self(), [&](auto &row) {
            row.collection = collection;
        });
    } else {
        check(false, "Record already exists");
    }
}

/**
 * Removes a collection from tracking
 * 
 * @param {name} collection - collection to remove from tracking
 * @auth - the contract itself
 */
ACTION burn4scoring::remallow(
    name collection
) {
    // Check for contract auth
    require_auth(get_self());

    // Get allowed table
    allowed_t allowed(get_self(), get_self().value);
    auto iterator = allowed.find(collection.value);

    // Remove if exist
    check(iterator != allowed.end(), "Record does not exist");
    allowed.erase(iterator);
}

/**
 * Adds a wallet to exclude
 * 
 * @param {name} wallet - wallet to exclude
 * @auth - the contract itself
 */
ACTION burn4scoring::addexclude(
    name wallet
) {
    // Check for contract auth
    require_auth(get_self());

    // Get excluded table
    excluded_t excluded(get_self(), get_self().value);
    auto iterator = excluded.find(wallet.value);

    // Add if not exist
    if (iterator == excluded.end()) {
        excluded.emplace(get_self(), [&](auto &row) {
            row.wallet = wallet;
        });
    } else {
        check(false, "Record already exists");
    }
}

/**
 * Removes a wallet from exclusion
 * 
 * @param {name} wallet - wallet to remove from exclusion
 * @auth - the contract itself
 */
ACTION burn4scoring::remexclude(
    name wallet
) {
    // Check for contract auth
    require_auth(get_self());

    // Get excluded table
    excluded_t excluded(get_self(), get_self().value);
    auto iterator = excluded.find(wallet.value);

    // Remove if exist
    check(iterator != excluded.end(), "Record does not exist");
    excluded.erase(iterator);
}

/**
 * Upserts a score entry
 * 
 * @param {name} wallet - wallet of score
 * @param {uint32_t} score - score to assign
 * @auth - the contract itself
 */
ACTION burn4scoring::upsertscore(
    name wallet,
    uint32_t score
) {
    // Check for contract auth
    require_auth(get_self());

    // Get score table
    score_t scores(get_self(), get_self().value);
    auto iterator = scores.find(wallet.value);

    // Modify if exist, otherwise add new row
    if (iterator == scores.end()) {
        scores.emplace(get_self(), [&](auto &row) {
            row.wallet = wallet;
            row.score = score;
        });
    } else {
        scores.modify(iterator, get_self(), [&](auto &row) {
            row.score = score;
        });
    }
}

/**
 * Removes a score entry
 * 
 * @param {name} wallet - wallet to remove from exclusion
 * @auth - the contract itself
 */
ACTION burn4scoring::remscore(
    name wallet
) {
    // Check for contract auth
    require_auth(get_self());

    // Get score table
    score_t scores(get_self(), get_self().value);
    auto iterator = scores.find(wallet.value);

    // Remove if exist
    check(iterator != scores.end(), "Record does not exist");
    scores.erase(iterator);
}

/**
 * Drops the score table
 * 
 * @auth - the contract itself
 */
ACTION burn4scoring::dropscore() {
    // Check for contract auth
    require_auth(get_self());

    // Get score table
    score_t scores(get_self(), get_self().value);
    auto iterator = scores.begin();

    // Remove all rows
    while (iterator != scores.end()) {
        iterator = scores.erase(iterator);
    }
}

/**
 * Upserts a token redemption entry
 * 
 * @param {int32_t} template_id - template to add for token redemption
 * @param {vector<extended_asset>} quantities - tokens to be redeemed
 * @auth - the contract itself
 */
ACTION burn4scoring::upsertredeem(
    int32_t template_id,
    std::vector<extended_asset> quantities
) {
    // Get contract auth
    require_auth(get_self());

    // Get redeem table
    redeem_t redeem(get_self(), get_self().value);
    auto iterator = redeem.find(template_id);

    // Modify if exist, otherwise add new row
    if (iterator == redeem.end()) {
        redeem.emplace(get_self(), [&](auto &row) {
            row.template_id = template_id;
            row.quantities = quantities;
        });
    } else {
        redeem.modify(iterator, get_self(), [&](auto &row) {
            row.quantities = quantities;
        });
    }
}

/**
 * Removes a template from token redemption
 * 
 * @param {int32_t} template_id - template to remove from token redemption
 * @auth - the contract itself
 */
ACTION burn4scoring::remredeem(
    int32_t template_id
) {
    // Get contract auth
    require_auth(get_self());

    // Get redeem table
    redeem_t redeem(get_self(), get_self().value);
    auto iterator = redeem.find(template_id);

    // Remove if exist
    check(iterator != redeem.end(), "Record does not exist");
    redeem.erase(iterator);
}

/**
 * Helper function for transferring tokens
 * 
 * @param {name} receiver - wallet to receive tokens
 * @param {name} contract - token contract
 * @param {asset} amount - token amount
 * @param {string} memo - transfer memo
 * @auth - the contract itself
 */
void burn4scoring::transfer_token(
    name receiver,
    name contract,
    asset amount,
    std::string memo
) {
    action(
        permission_level{get_self(), name("active")},
        contract,
        name("transfer"),
        std::make_tuple(
            get_self(),
            receiver,
            amount,
            memo
        )
    ).send();
}

/**
 * Asset burn callback
 */
void burn4scoring::redeem(
    name asset_owner,
    uint64_t asset_id,
    name collection_name,
    name schema_name,
    int32_t template_id,
    std::vector<asset> backed_tokens,
    atomicassets::ATTRIBUTE_MAP old_immutable_data,
    atomicassets::ATTRIBUTE_MAP old_mutable_data,
    name asset_ram_payer
) {
    // Get redeem table
    redeem_t redeem(get_self(), get_self().value);
    auto redeem_itr = redeem.find(template_id);
    // Send tokens if template matches
    if (redeem_itr != redeem.end()) {
        auto tokens = redeem_itr->quantities;
        for (size_t i = 0; i < tokens.size(); i++) {
            transfer_token(asset_owner, tokens[i].contract, tokens[i].quantity, ("Token redemption for asset " + std::to_string(asset_id)).c_str());
        }
    }

    // Get excluded table
    excluded_t excluded(get_self(), get_self().value);
    auto excluded_itr = excluded.find(asset_owner.value);
    // Skip further processing if wallet excluded
    if (excluded_itr != excluded.end()) {
        return;
    }

    // Get allowed table
    allowed_t allowed(get_self(), get_self().value);
    auto allowed_itr = allowed.find(collection_name.value);
    // Skip further processing if collection not tracked
    if (allowed_itr == allowed.end()) {
        return;
    }

    // Get atomicassets templates table
    atomicassets::templates_t templates = atomicassets::get_templates(collection_name);
    auto template_itr = templates.find(template_id);
    // Get template issued supply
    uint32_t supply = template_itr->issued_supply;

    // Get config table
    config_t configs(get_self(), get_self().value);
    // Determine score using issued supply as limit (inclusive)
    auto config_itr = configs.lower_bound(supply);
    uint32_t score_delta = 1;
    if (config_itr != configs.end()) {
        score_delta = config_itr->score;
    }

    // Get score table
    score_t scores(get_self(), get_self().value);
    auto score_itr = scores.find(asset_owner.value);
    // Add to score if exist, otherwise add new row
    if (score_itr == scores.end()) {
        scores.emplace(get_self(), [&](auto &row) {
            row.wallet = asset_owner;
            row.score = score_delta;
        });
    } else {
        score_delta += score_itr->score;
        scores.modify(score_itr, get_self(), [&](auto &row) {
            row.score = score_delta;
        });
    }
}
