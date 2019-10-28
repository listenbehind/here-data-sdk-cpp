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

#include <gmock/gmock.h>
#include <matchers/NetworkUrlMatchers.h>
#include <mocks/NetworkMock.h>
#include <olp/core/cache/DefaultCache.h>
#include <olp/core/client/Condition.h>
#include <olp/core/client/HRN.h>
#include <olp/core/client/OlpClientSettingsFactory.h>
#include <olp/core/http/Network.h>
#include <olp/core/logging/Log.h>
#include <olp/core/porting/make_unique.h>
#include <olp/dataservice/read/PartitionsRequest.h>
#include <olp/dataservice/read/VolatileLayerClient.h>

#include "HttpResponses.h"

namespace {

using namespace olp::dataservice::read;
using namespace testing;
using namespace olp::tests::common;

const auto kTimeout = std::chrono::seconds(5);

class DataserviceReadVolatileLayerClientTest : public ::testing::Test {
 protected:
  DataserviceReadVolatileLayerClientTest();
  ~DataserviceReadVolatileLayerClientTest();
  std::string GetTestCatalog();
  static std::string ApiErrorToString(const olp::client::ApiError& error);

  void SetUp() override;
  void TearDown() override;

 private:
  void SetUpCommonNetworkMockCalls();

 protected:
  olp::client::OlpClientSettings settings_;
  std::shared_ptr<olp::tests::common::NetworkMock> network_mock_;
};

DataserviceReadVolatileLayerClientTest::DataserviceReadVolatileLayerClientTest() = default;

DataserviceReadVolatileLayerClientTest::~DataserviceReadVolatileLayerClientTest() = default;

std::string DataserviceReadVolatileLayerClientTest::GetTestCatalog() {
  return "hrn:here:data:::hereos-internal-test-v2";
}

std::string DataserviceReadVolatileLayerClientTest::ApiErrorToString(
    const olp::client::ApiError& error) {
  std::ostringstream result_stream;
  result_stream << "ERROR: code: " << static_cast<int>(error.GetErrorCode())
                << ", status: " << error.GetHttpStatusCode()
                << ", message: " << error.GetMessage();
  return result_stream.str();
}

void DataserviceReadVolatileLayerClientTest::SetUp() {
  network_mock_ = std::make_shared<NetworkMock>();
  settings_ = olp::client::OlpClientSettings();
  settings_.network_request_handler = network_mock_;
  olp::cache::CacheSettings cache_settings;
  settings_.cache =
      olp::client::OlpClientSettingsFactory::CreateDefaultCache(cache_settings);

  SetUpCommonNetworkMockCalls();
}

void DataserviceReadVolatileLayerClientTest::TearDown() {
  ::testing::Mock::VerifyAndClearExpectations(network_mock_.get());
  network_mock_.reset();
}

void DataserviceReadVolatileLayerClientTest::SetUpCommonNetworkMockCalls() {
  ON_CALL(*network_mock_, Send(IsGetRequest(URL_LOOKUP_CONFIG), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_LOOKUP_CONFIG));

  ON_CALL(*network_mock_, Send(IsGetRequest(URL_CONFIG), _, _, _, _))
      .WillByDefault(ReturnHttpResponse(
          olp::http::NetworkResponse().WithStatus(200), HTTP_RESPONSE_CONFIG));

  ON_CALL(*network_mock_, Send(IsGetRequest(URL_LOOKUP_METADATA), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_LOOKUP_METADATA));

  ON_CALL(*network_mock_,
          Send(IsGetRequest(URL_LATEST_CATALOG_VERSION), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_LATEST_CATALOG_VERSION));

  ON_CALL(*network_mock_, Send(IsGetRequest(URL_LAYER_VERSIONS), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_LAYER_VERSIONS));

  ON_CALL(*network_mock_, Send(IsGetRequest(URL_PARTITIONS), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_PARTITIONS));

  ON_CALL(*network_mock_, Send(IsGetRequest(URL_LOOKUP_QUERY), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_LOOKUP_QUERY));

  ON_CALL(*network_mock_,
          Send(IsGetRequest(URL_QUERY_PARTITION_269), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_PARTITION_269));

  ON_CALL(*network_mock_, Send(IsGetRequest(URL_LOOKUP_BLOB), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_LOOKUP_BLOB));

  ON_CALL(*network_mock_, Send(IsGetRequest(URL_BLOB_DATA_269), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_BLOB_DATA_269));

  ON_CALL(*network_mock_, Send(IsGetRequest(URL_PARTITION_3), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_PARTITION_3));

  ON_CALL(*network_mock_,
          Send(IsGetRequest(URL_LOOKUP_VOLATILE_BLOB), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_LOOKUP_VOLATILE_BLOB));

  ON_CALL(*network_mock_, Send(IsGetRequest(URL_LAYER_VERSIONS_V2), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_LAYER_VERSIONS_V2));

  ON_CALL(*network_mock_, Send(IsGetRequest(URL_PARTITIONS_V2), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_PARTITIONS_V2));

  ON_CALL(*network_mock_,
          Send(IsGetRequest(URL_QUERY_PARTITION_269_V2), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_PARTITION_269_V2));

  ON_CALL(*network_mock_, Send(IsGetRequest(URL_BLOB_DATA_269_V2), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_BLOB_DATA_269_V2));

  ON_CALL(*network_mock_,
          Send(IsGetRequest(URL_QUERY_PARTITION_269_V10), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(400),
                             HTTP_RESPONSE_INVALID_VERSION_V10));

  ON_CALL(*network_mock_,
          Send(IsGetRequest(URL_QUERY_PARTITION_269_VN1), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(400),
                             HTTP_RESPONSE_INVALID_VERSION_VN1));

  ON_CALL(*network_mock_,
          Send(IsGetRequest(URL_LAYER_VERSIONS_V10), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(400),
                             HTTP_RESPONSE_INVALID_VERSION_V10));

  ON_CALL(*network_mock_,
          Send(IsGetRequest(URL_LAYER_VERSIONS_VN1), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(400),
                             HTTP_RESPONSE_INVALID_VERSION_VN1));

  ON_CALL(*network_mock_, Send(IsGetRequest(URL_CONFIG_V2), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_CONFIG_V2));

  ON_CALL(*network_mock_, Send(IsGetRequest(URL_QUADKEYS_23618364), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_QUADKEYS_23618364));

  ON_CALL(*network_mock_, Send(IsGetRequest(URL_QUADKEYS_1476147), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_QUADKEYS_1476147));

  ON_CALL(*network_mock_, Send(IsGetRequest(URL_QUADKEYS_5904591), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_QUADKEYS_5904591));

  ON_CALL(*network_mock_, Send(IsGetRequest(URL_QUADKEYS_369036), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_QUADKEYS_369036));

  ON_CALL(*network_mock_,
          Send(IsGetRequest(URL_BLOB_DATA_PREFETCH_1), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_BLOB_DATA_PREFETCH_1));

  ON_CALL(*network_mock_,
          Send(IsGetRequest(URL_BLOB_DATA_PREFETCH_2), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_BLOB_DATA_PREFETCH_2));

  ON_CALL(*network_mock_,
          Send(IsGetRequest(URL_BLOB_DATA_PREFETCH_3), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_BLOB_DATA_PREFETCH_3));

  ON_CALL(*network_mock_,
          Send(IsGetRequest(URL_BLOB_DATA_PREFETCH_4), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_BLOB_DATA_PREFETCH_4));

  ON_CALL(*network_mock_,
          Send(IsGetRequest(URL_BLOB_DATA_PREFETCH_5), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_BLOB_DATA_PREFETCH_5));

  ON_CALL(*network_mock_,
          Send(IsGetRequest(URL_BLOB_DATA_PREFETCH_6), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_BLOB_DATA_PREFETCH_6));

  ON_CALL(*network_mock_,
          Send(IsGetRequest(URL_BLOB_DATA_PREFETCH_7), _, _, _, _))
      .WillByDefault(
          ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                             HTTP_RESPONSE_BLOB_DATA_PREFETCH_7));

  // Catch any non-interesting network calls that don't need to be verified
  EXPECT_CALL(*network_mock_, Send(_, _, _, _, _)).Times(testing::AtLeast(0));
}

TEST_F(DataserviceReadVolatileLayerClientTest, GetPartitions) {
  olp::client::HRN hrn(GetTestCatalog());

  EXPECT_CALL(*network_mock_, Send(IsGetRequest(URL_CONFIG), _, _, _, _))
      .Times(1);

  olp::dataservice::read::VolatileLayerClient client(hrn, "testlayer",
                                                     settings_);

  auto request = olp::dataservice::read::PartitionsRequest();
  VolatileLayerClient::PartitionsResponse partitions_response;
  olp::client::Condition condition;
  client.GetPartitions(request,
                       [&](VolatileLayerClient::PartitionsResponse response) {
                         partitions_response = std::move(response);
                         condition.Notify();
                       });

  ASSERT_TRUE(condition.Wait(kTimeout));

  ASSERT_TRUE(partitions_response.IsSuccessful())
      << ApiErrorToString(partitions_response.GetError());
  ASSERT_EQ(4u, partitions_response.GetResult().GetPartitions().size());
}

TEST_F(DataserviceReadVolatileLayerClientTest, GetEmptyPartitions) {
  olp::client::HRN hrn(GetTestCatalog());

  EXPECT_CALL(*network_mock_, Send(IsGetRequest(URL_PARTITIONS), _, _, _, _))
      .WillOnce(ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                                   HTTP_RESPONSE_EMPTY_PARTITIONS));

  olp::dataservice::read::VolatileLayerClient client(hrn, "testlayer",
                                                     settings_);

  auto request = olp::dataservice::read::PartitionsRequest();
  VolatileLayerClient::PartitionsResponse partitions_response;
  olp::client::Condition condition;
  client.GetPartitions(request,
                       [&](VolatileLayerClient::PartitionsResponse response) {
                         partitions_response = std::move(response);
                         condition.Notify();
                       });

  ASSERT_TRUE(condition.Wait(kTimeout));

  ASSERT_TRUE(partitions_response.IsSuccessful())
      << ApiErrorToString(partitions_response.GetError());
  ASSERT_EQ(0u, partitions_response.GetResult().GetPartitions().size());
}

TEST_F(DataserviceReadVolatileLayerClientTest, GetVolatilePartitions) {
  olp::client::HRN hrn(GetTestCatalog());

  EXPECT_CALL(*network_mock_,
              Send(IsGetRequest(URL_LATEST_CATALOG_VERSION), _, _, _, _))
      .Times(0);

  EXPECT_CALL(*network_mock_,
              Send(IsGetRequest("https://metadata.data.api.platform.here.com/"
                                "metadata/v1/catalogs/hereos-internal-test-v2/"
                                "layers/testlayer_volatile/partitions"),
                   _, _, _, _))
      .WillOnce(ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                                   HTTP_RESPONSE_PARTITIONS_V2));

  olp::dataservice::read::VolatileLayerClient client(hrn, "testlayer_volatile",
                                                     settings_);

  auto request = olp::dataservice::read::PartitionsRequest();
  VolatileLayerClient::PartitionsResponse partitions_response;
  olp::client::Condition condition;
  client.GetPartitions(request,
                       [&](VolatileLayerClient::PartitionsResponse response) {
                         partitions_response = std::move(response);
                         condition.Notify();
                       });

  ASSERT_TRUE(condition.Wait(kTimeout));

  ASSERT_TRUE(partitions_response.IsSuccessful())
      << ApiErrorToString(partitions_response.GetError());
  ASSERT_EQ(1u, partitions_response.GetResult().GetPartitions().size());
}

TEST_F(DataserviceReadVolatileLayerClientTest, GetPartitions429Error) {
  olp::client::HRN hrn(GetTestCatalog());

  {
    testing::InSequence s;

    EXPECT_CALL(*network_mock_, Send(IsGetRequest(URL_PARTITIONS), _, _, _, _))
        .Times(2)
        .WillRepeatedly(
            ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(429),
                               "Server busy at the moment."));

    EXPECT_CALL(*network_mock_, Send(IsGetRequest(URL_PARTITIONS), _, _, _, _))
        .Times(1);
  }

  olp::client::RetrySettings retry_settings;
  retry_settings.retry_condition =
      [](const olp::client::HttpResponse& response) {
        return 429 == response.status;
      };
  settings_.retry_settings = retry_settings;
  olp::dataservice::read::VolatileLayerClient client(hrn, "testlayer",
                                                     settings_);

  auto request = olp::dataservice::read::PartitionsRequest();
  VolatileLayerClient::PartitionsResponse partitions_response;
  olp::client::Condition condition;
  client.GetPartitions(request,
                       [&](VolatileLayerClient::PartitionsResponse response) {
                         partitions_response = std::move(response);
                         condition.Notify();
                       });

  ASSERT_TRUE(condition.Wait(kTimeout));

  ASSERT_TRUE(partitions_response.IsSuccessful())
      << ApiErrorToString(partitions_response.GetError());
  ASSERT_EQ(4u, partitions_response.GetResult().GetPartitions().size());
}

TEST_F(DataserviceReadVolatileLayerClientTest, ApiLookup429) {
  olp::client::HRN hrn(GetTestCatalog());

  {
    testing::InSequence s;

    EXPECT_CALL(*network_mock_,
                Send(IsGetRequest(URL_LOOKUP_METADATA), _, _, _, _))
        .Times(2)
        .WillRepeatedly(
            ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(429),
                               "Server busy at the moment."));

    EXPECT_CALL(*network_mock_,
                Send(IsGetRequest(URL_LOOKUP_METADATA), _, _, _, _))
        .Times(1);
  }

  olp::client::RetrySettings retry_settings;
  retry_settings.retry_condition =
      [](const olp::client::HttpResponse& response) {
        return 429 == response.status;
      };
  settings_.retry_settings = retry_settings;
  olp::dataservice::read::VolatileLayerClient client(hrn, "testlayer",
                                                     settings_);

  auto request = olp::dataservice::read::PartitionsRequest();
  VolatileLayerClient::PartitionsResponse partitions_response;
  olp::client::Condition condition;
  client.GetPartitions(request,
                       [&](VolatileLayerClient::PartitionsResponse response) {
                         partitions_response = std::move(response);
                         condition.Notify();
                       });

  ASSERT_TRUE(condition.Wait(kTimeout));

  ASSERT_TRUE(partitions_response.IsSuccessful())
      << ApiErrorToString(partitions_response.GetError());
  ASSERT_EQ(4u, partitions_response.GetResult().GetPartitions().size());
}

TEST_F(DataserviceReadVolatileLayerClientTest, GetPartitionsForInvalidLayer) {
  olp::client::HRN hrn(GetTestCatalog());

  olp::dataservice::read::VolatileLayerClient client(hrn, "InvalidLayer",
                                                     settings_);

  auto request = olp::dataservice::read::PartitionsRequest();
  VolatileLayerClient::PartitionsResponse partitions_response;
  olp::client::Condition condition;
  client.GetPartitions(request,
                       [&](VolatileLayerClient::PartitionsResponse response) {
                         partitions_response = std::move(response);
                         condition.Notify();
                       });

  ASSERT_TRUE(condition.Wait(kTimeout));

  ASSERT_FALSE(partitions_response.IsSuccessful())
      << ApiErrorToString(partitions_response.GetError());
  ASSERT_EQ(olp::client::ErrorCode::InvalidArgument,
            partitions_response.GetError().GetErrorCode());
}

TEST_F(DataserviceReadVolatileLayerClientTest, GetPartitionsGarbageResponse) {
  olp::client::HRN hrn(GetTestCatalog());

  EXPECT_CALL(*network_mock_,
              Send(IsGetRequest(URL_LOOKUP_METADATA), _, _, _, _))
      .WillOnce(ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(200),
                                   R"jsonString(kd3sdf\)jsonString"));

  olp::dataservice::read::VolatileLayerClient client(hrn, "testlayer",
                                                     settings_);

  auto request = olp::dataservice::read::PartitionsRequest();
  VolatileLayerClient::PartitionsResponse partitions_response;
  olp::client::Condition condition;
  client.GetPartitions(request,
                       [&](VolatileLayerClient::PartitionsResponse response) {
                         partitions_response = std::move(response);
                         condition.Notify();
                       });

  ASSERT_TRUE(condition.Wait(kTimeout));

  ASSERT_FALSE(partitions_response.IsSuccessful());
  ASSERT_EQ(olp::client::ErrorCode::ServiceUnavailable,
            partitions_response.GetError().GetErrorCode());
}

TEST_F(DataserviceReadVolatileLayerClientTest, GetPartitionsCancelLookupMetadata) {
  olp::client::HRN hrn(GetTestCatalog());

  // Setup the expected calls :
  auto wait_for_cancel = std::make_shared<std::promise<void>>();
  auto pause_for_cancel = std::make_shared<std::promise<void>>();

  olp::http::RequestId request_id;
  NetworkCallback send_mock;
  CancelCallback cancel_mock;

  std::tie(request_id, send_mock, cancel_mock) = GenerateNetworkMockActions(
      wait_for_cancel, pause_for_cancel, {200, HTTP_RESPONSE_LOOKUP_METADATA});

  EXPECT_CALL(*network_mock_,
              Send(IsGetRequest(URL_LOOKUP_METADATA), _, _, _, _))
      .Times(1)
      .WillOnce(testing::Invoke(std::move(send_mock)));

  EXPECT_CALL(*network_mock_, Cancel(request_id))
      .WillOnce(testing::Invoke(std::move(cancel_mock)));

  EXPECT_CALL(*network_mock_,
              Send(IsGetRequest(URL_LATEST_CATALOG_VERSION), _, _, _, _))
      .Times(0);

  std::promise<VolatileLayerClient::PartitionsResponse> promise;
  auto callback = [&promise](VolatileLayerClient::PartitionsResponse response) {
    promise.set_value(response);
  };

  VolatileLayerClient client(hrn, "testlayer", settings_);

  auto request = olp::dataservice::read::PartitionsRequest();
  auto cancel_token = client.GetPartitions(request, callback);

  wait_for_cancel->get_future().get();  // wait for handler to get the request
  cancel_token.cancel();
  pause_for_cancel->set_value();  // unblock the handler
  VolatileLayerClient::PartitionsResponse partitions_response =
      promise.get_future().get();

  ASSERT_FALSE(partitions_response.IsSuccessful())
      << ApiErrorToString(partitions_response.GetError());
  ASSERT_EQ(static_cast<int>(olp::http::ErrorCode::CANCELLED_ERROR),
            partitions_response.GetError().GetHttpStatusCode());
  ASSERT_EQ(olp::client::ErrorCode::Cancelled,
            partitions_response.GetError().GetErrorCode());
}

TEST_F(DataserviceReadVolatileLayerClientTest, GetPartitionsCancelLatestCatalogVersion) {
  olp::client::HRN hrn(GetTestCatalog());

  // Setup the expected calls :
  auto wait_for_cancel = std::make_shared<std::promise<void>>();
  auto pause_for_cancel = std::make_shared<std::promise<void>>();

  olp::http::RequestId request_id;
  NetworkCallback send_mock;
  CancelCallback cancel_mock;

  std::tie(request_id, send_mock, cancel_mock) =
      GenerateNetworkMockActions(wait_for_cancel, pause_for_cancel,
                                 {200, HTTP_RESPONSE_LATEST_CATALOG_VERSION});

  EXPECT_CALL(*network_mock_,
              Send(IsGetRequest(URL_LATEST_CATALOG_VERSION), _, _, _, _))
      .Times(1)
      .WillOnce(testing::Invoke(std::move(send_mock)));

  EXPECT_CALL(*network_mock_, Cancel(request_id))
      .WillOnce(testing::Invoke(std::move(cancel_mock)));

  EXPECT_CALL(*network_mock_,
              Send(IsGetRequest(URL_LAYER_VERSIONS), _, _, _, _))
      .Times(0);

  std::promise<VolatileLayerClient::PartitionsResponse> promise;
  auto callback = [&promise](VolatileLayerClient::PartitionsResponse response) {
    promise.set_value(response);
  };

  VolatileLayerClient client(hrn, "testlayer", settings_);

  auto request = olp::dataservice::read::PartitionsRequest();
  auto cancel_token = client.GetPartitions(request, callback);

  wait_for_cancel->get_future().get();  // wait for handler to get the request
  cancel_token.cancel();
  pause_for_cancel->set_value();  // unblock the handler
  VolatileLayerClient::PartitionsResponse partitions_response =
      promise.get_future().get();

  ASSERT_FALSE(partitions_response.IsSuccessful())
      << ApiErrorToString(partitions_response.GetError());
  ASSERT_EQ(static_cast<int>(olp::http::ErrorCode::CANCELLED_ERROR),
            partitions_response.GetError().GetHttpStatusCode())
      << ApiErrorToString(partitions_response.GetError());
  ASSERT_EQ(olp::client::ErrorCode::Cancelled,
            partitions_response.GetError().GetErrorCode())
      << ApiErrorToString(partitions_response.GetError());
}

TEST_F(DataserviceReadVolatileLayerClientTest, DISABLED_GetPartitionsCancelLayerVersions) {
  olp::client::HRN hrn(GetTestCatalog());

  // Setup the expected calls :
  auto wait_for_cancel = std::make_shared<std::promise<void>>();
  auto pause_for_cancel = std::make_shared<std::promise<void>>();

  olp::http::RequestId request_id;
  NetworkCallback send_mock;
  CancelCallback cancel_mock;

  std::tie(request_id, send_mock, cancel_mock) = GenerateNetworkMockActions(
      wait_for_cancel, pause_for_cancel, {200, HTTP_RESPONSE_LAYER_VERSIONS});

  EXPECT_CALL(*network_mock_,
              Send(IsGetRequest(URL_LAYER_VERSIONS), _, _, _, _))
      .Times(1)
      .WillOnce(testing::Invoke(std::move(send_mock)));

  EXPECT_CALL(*network_mock_, Cancel(request_id))
      .WillOnce(testing::Invoke(std::move(cancel_mock)));

  EXPECT_CALL(*network_mock_, Send(IsGetRequest(URL_PARTITIONS), _, _, _, _))
      .Times(0);

  std::promise<VolatileLayerClient::PartitionsResponse> promise;
  auto callback = [&promise](VolatileLayerClient::PartitionsResponse response) {
    promise.set_value(response);
  };

  VolatileLayerClient client(hrn, "testlayer", settings_);

  auto request = olp::dataservice::read::PartitionsRequest();
  auto cancel_token = client.GetPartitions(request, callback);

  wait_for_cancel->get_future().get();  // wait for handler to get the request
  cancel_token.cancel();
  pause_for_cancel->set_value();  // unblock the handler
  VolatileLayerClient::PartitionsResponse partitions_response =
      promise.get_future().get();

  ASSERT_FALSE(partitions_response.IsSuccessful())
      << ApiErrorToString(partitions_response.GetError());
  ASSERT_EQ(static_cast<int>(olp::http::ErrorCode::CANCELLED_ERROR),
            partitions_response.GetError().GetHttpStatusCode())
      << ApiErrorToString(partitions_response.GetError());
  ASSERT_EQ(olp::client::ErrorCode::Cancelled,
            partitions_response.GetError().GetErrorCode())
      << ApiErrorToString(partitions_response.GetError());
}

TEST_F(DataserviceReadVolatileLayerClientTest, GetPartitionsCacheOnly) {
  olp::client::HRN hrn(GetTestCatalog());

  EXPECT_CALL(*network_mock_, Send(IsGetRequest(URL_PARTITIONS), _, _, _, _))
      .Times(0);

  olp::dataservice::read::VolatileLayerClient client(hrn, "testlayer_volatile",
                                                     settings_);

  auto request = olp::dataservice::read::PartitionsRequest().WithFetchOption(
      FetchOptions::CacheOnly);
  VolatileLayerClient::PartitionsResponse partitions_response;
  olp::client::Condition condition;
  client.GetPartitions(request,
                       [&](VolatileLayerClient::PartitionsResponse response) {
                         partitions_response = std::move(response);
                         condition.Notify();
                       });

  ASSERT_TRUE(condition.Wait(kTimeout));

  ASSERT_FALSE(partitions_response.IsSuccessful())
      << ApiErrorToString(partitions_response.GetError());
}

TEST_F(DataserviceReadVolatileLayerClientTest, GetPartitionsOnlineOnly) {
  olp::client::HRN hrn(GetTestCatalog());

  {
    testing::InSequence s;

    EXPECT_CALL(*network_mock_, Send(IsGetRequest(URL_CONFIG), _, _, _, _))
        .Times(1);

    EXPECT_CALL(*network_mock_, Send(IsGetRequest(URL_CONFIG), _, _, _, _))
        .WillOnce(
            ReturnHttpResponse(olp::http::NetworkResponse().WithStatus(429),
                               "Server busy at the moment."));
  }

  olp::dataservice::read::VolatileLayerClient client(hrn, "testlayer",
                                                     settings_);

  auto request = olp::dataservice::read::PartitionsRequest().WithFetchOption(
      FetchOptions::OnlineOnly);
  {
    VolatileLayerClient::PartitionsResponse partitions_response;
    olp::client::Condition condition;
    client.GetPartitions(request,
                         [&](VolatileLayerClient::PartitionsResponse response) {
                           partitions_response = std::move(response);
                           condition.Notify();
                         });

    ASSERT_TRUE(condition.Wait(kTimeout));

    ASSERT_TRUE(partitions_response.IsSuccessful())
        << ApiErrorToString(partitions_response.GetError());
    ASSERT_EQ(4u, partitions_response.GetResult().GetPartitions().size());
  }

  {
    VolatileLayerClient::PartitionsResponse partitions_response;
    olp::client::Condition condition;
    client.GetPartitions(request,
                         [&](VolatileLayerClient::PartitionsResponse response) {
                           partitions_response = std::move(response);
                           condition.Notify();
                         });

    ASSERT_TRUE(condition.Wait(kTimeout));
    // Should fail despite valid cache entry
    ASSERT_FALSE(partitions_response.IsSuccessful())
        << ApiErrorToString(partitions_response.GetError());
  }
}

TEST_F(DataserviceReadVolatileLayerClientTest, DISABLED_GetPartitionsCacheWithUpdate) {
  olp::logging::Log::setLevel(olp::logging::Level::Trace);

  olp::client::HRN hrn(GetTestCatalog());

  auto wait_to_start_signal = std::make_shared<std::promise<void>>();
  auto pre_callback_wait = std::make_shared<std::promise<void>>();
  pre_callback_wait->set_value();
  auto wait_for_end_signal = std::make_shared<std::promise<void>>();

  olp::http::RequestId request_id;
  NetworkCallback send_mock;
  CancelCallback cancel_mock;

  std::tie(request_id, send_mock, cancel_mock) = GenerateNetworkMockActions(
      wait_to_start_signal, pre_callback_wait, {200, HTTP_RESPONSE_PARTITIONS},
      wait_for_end_signal);

  EXPECT_CALL(*network_mock_, Send(IsGetRequest(URL_PARTITIONS), _, _, _, _))
      .Times(1)
      .WillOnce(testing::Invoke(std::move(send_mock)));

  EXPECT_CALL(*network_mock_, Cancel(request_id))
      .WillOnce(testing::Invoke(std::move(cancel_mock)));

  olp::dataservice::read::VolatileLayerClient client(hrn, "testlayer",
                                                     settings_);
  auto request = olp::dataservice::read::PartitionsRequest().WithFetchOption(
      FetchOptions::CacheWithUpdate);
  {
    VolatileLayerClient::PartitionsResponse partitions_response;
    olp::client::Condition condition;
    client.GetPartitions(request,
                         [&](VolatileLayerClient::PartitionsResponse response) {
                           partitions_response = std::move(response);
                           condition.Notify();
                         });

    ASSERT_TRUE(condition.Wait(kTimeout));

    // Request 1 return. Cached value (nothing)
    ASSERT_FALSE(partitions_response.IsSuccessful())
        << ApiErrorToString(partitions_response.GetError());
  }

  {
    // Request 2 to check there is a cached value.
    wait_for_end_signal->get_future().get();
    request.WithFetchOption(CacheOnly);
    VolatileLayerClient::PartitionsResponse partitions_response;
    olp::client::Condition condition;
    client.GetPartitions(request,
                         [&](VolatileLayerClient::PartitionsResponse response) {
                           partitions_response = std::move(response);
                           condition.Notify();
                         });

    ASSERT_TRUE(condition.Wait(kTimeout));
    // Cache should be available here.
    ASSERT_TRUE(partitions_response.IsSuccessful())
        << ApiErrorToString(partitions_response.GetError());
  }
}

TEST_F(DataserviceReadVolatileLayerClientTest, GetPartitions403CacheClear) {
  olp::client::HRN hrn(GetTestCatalog());
  olp::dataservice::read::VolatileLayerClient client(hrn, "testlayer",
                                                     settings_);
  {
    testing::InSequence s;
    EXPECT_CALL(*network_mock_, Send(IsGetRequest(URL_PARTITIONS), _, _, _, _))
        .Times(1);
    EXPECT_CALL(*network_mock_, Send(IsGetRequest(URL_PARTITIONS), _, _, _, _))
        .WillOnce(ReturnHttpResponse(
            olp::http::NetworkResponse().WithStatus(403), HTTP_RESPONSE_403));
  }

  // Populate cache
  auto request = olp::dataservice::read::PartitionsRequest();
  VolatileLayerClient::PartitionsResponse partitions_response;
  olp::client::Condition condition;
  client.GetPartitions(request,
                       [&](VolatileLayerClient::PartitionsResponse response) {
                         partitions_response = std::move(response);
                         condition.Notify();
                       });

  ASSERT_TRUE(condition.Wait(kTimeout));
  ASSERT_TRUE(partitions_response.IsSuccessful());

  // Receive 403
  request.WithFetchOption(OnlineOnly);
  client.GetPartitions(request,
                       [&](VolatileLayerClient::PartitionsResponse response) {
                         partitions_response = std::move(response);
                         condition.Notify();
                       });
  ASSERT_TRUE(condition.Wait(kTimeout));
  ASSERT_FALSE(partitions_response.IsSuccessful());
  ASSERT_EQ(403, partitions_response.GetError().GetHttpStatusCode());

  // Check for cached response
  request.WithFetchOption(CacheOnly);
  client.GetPartitions(request,
                       [&](VolatileLayerClient::PartitionsResponse response) {
                         partitions_response = std::move(response);
                         condition.Notify();
                       });
  ASSERT_TRUE(condition.Wait(kTimeout));
  ASSERT_FALSE(partitions_response.IsSuccessful());
}

}  // namespace