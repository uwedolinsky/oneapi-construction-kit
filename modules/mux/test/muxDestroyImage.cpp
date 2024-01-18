// Copyright (C) Codeplay Software Limited
//
// Licensed under the Apache License, Version 2.0 (the "License") with LLVM
// Exceptions; you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://github.com/codeplaysoftware/oneapi-construction-kit/blob/main/LICENSE.txt
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations
// under the License.
//
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "common.h"

struct muxDestroyImageTest : public DeviceTest {};

INSTANTIATE_DEVICE_TEST_SUITE_P(muxDestroyImageTest);

TEST_P(muxDestroyImageTest, Default) {
  if (device->info->image_support) {
    const mux_image_type_e type = mux_image_type_1d;
    const mux_allocation_type_e allocation_type =
        (mux_allocation_capabilities_alloc_device &
         device->info->allocation_capabilities)
            ? mux_allocation_type_alloc_device
            : mux_allocation_type_alloc_host;
    uint32_t out_count = 0;
    ASSERT_SUCCESS(muxGetSupportedImageFormats(device, type, allocation_type, 0,
                                               nullptr, &out_count));

    std::vector<mux_image_format_e> format(out_count);

    ASSERT_SUCCESS(muxGetSupportedImageFormats(device, type, allocation_type,
                                               out_count, &format[0], nullptr));
    // For each of the supported image formats on this device we test creation
    // of an image
    for (uint64_t j = 0; j < out_count; j++) {
      mux_image_t outimage;
      ASSERT_SUCCESS(muxCreateImage(device, type, format[j], 16, 1, 1, 0, 0, 0,
                                    allocator, &outimage));
      muxDestroyImage(device, outimage, allocator);
    }
  }
}

TEST_P(muxDestroyImageTest, MalformedDevice) {
  if (device->info->image_support) {
    const mux_image_type_e type = mux_image_type_1d;
    const mux_allocation_type_e allocation_type =
        (mux_allocation_capabilities_alloc_device &
         device->info->allocation_capabilities)
            ? mux_allocation_type_alloc_device
            : mux_allocation_type_alloc_host;
    uint32_t out_count = 0;
    ASSERT_SUCCESS(muxGetSupportedImageFormats(device, type, allocation_type, 0,
                                               nullptr, &out_count));

    std::vector<mux_image_format_e> format(out_count);

    ASSERT_SUCCESS(muxGetSupportedImageFormats(device, type, allocation_type,
                                               out_count, &format[0], nullptr));
    // For each of the supported image formats on this device we test creation
    // of an image
    for (uint64_t j = 0; j < out_count; j++) {
      mux_image_t outimage;
      ASSERT_SUCCESS(muxCreateImage(device, type, format[j], 16, 1, 1, 0, 0, 0,
                                    allocator, &outimage));
      muxDestroyImage(nullptr, outimage, allocator);
      muxDestroyImage(device, nullptr, allocator);
      muxDestroyImage(device, outimage, allocator);
    }
  }
}
