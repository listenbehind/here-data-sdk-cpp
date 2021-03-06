/*
 * Copyright (C) 2019 HERE Europe B.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 * License-Filename: LICENSE
 */

#pragma once

#include <memory>
#include <string>

#include <olp/core/client/ApiError.h>
#include <olp/core/client/ApiResponse.h>
#include "generated/model/Api.h"

namespace olp {
namespace client {
class OlpClient;
class CancellationContext;
}  // namespace client

namespace dataservice {
namespace read {
/**
 * @brief Api to search resource base urls.
 */
class ResourcesApi {
 public:
  using ApisResponse = client::ApiResponse<model::Apis, client::ApiError>;

  /**
   * @brief Call to resources service base urls.
   * @param client Instance of OlpClient used to make REST request.
   * @param hrn Full catalog name.
   * @param context A CancellationContext, which can be used to cancel any
   * pending request.
   *
   * @return The Apis response.
   */
  static ApisResponse GetApis(const client::OlpClient& client,
                              const std::string& hrn,
                              const client::CancellationContext& context);
};

}  // namespace read
}  // namespace dataservice
}  // namespace olp
