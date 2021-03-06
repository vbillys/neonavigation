/*
 * Copyright (c) 2014-2017, the neonavigation authors
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the copyright holder nor the names of its 
 *       contributors may be used to endorse or promote products derived from 
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <tf_projection.h>
#include <gtest/gtest.h>

void test_transform(
    const tf::StampedTransform proj2base,
    const tf::StampedTransform targ2proj,
    const tf::StampedTransform truth)
{
  tf_projection tfp;

  tf::StampedTransform result = tfp.project(proj2base, targ2proj);

  const float error_x = fabs(result.getOrigin().x() - truth.getOrigin().x());
  const float error_y = fabs(result.getOrigin().y() - truth.getOrigin().y());
  const float error_z = fabs(result.getOrigin().z() - truth.getOrigin().z());
  const tf::Quaternion error_q = result.getRotation() * truth.getRotation().inverse();
  ASSERT_LT(error_x, 0.001);
  ASSERT_LT(error_y, 0.001);
  ASSERT_LT(error_z, 0.001);
  ASSERT_LT(fabs(error_q.getAngle()), 0.001);
}

TEST(tf_projection_test, projection_test)
{
  test_transform(
      tf::StampedTransform(
          tf::Transform(tf::Quaternion(tf::Vector3(0.0, 0.0, 1.0), M_PI / 2.0),
                        tf::Vector3(1.0, 3.0, 10.0)),
          ros::Time(0),
          "map", "base_link"),
      // projected: t(1.0, 3.0, 0.0), r((0.0, 0.0, 1.0), M_PI/2.0)
      tf::StampedTransform(
          tf::Transform(tf::Quaternion(tf::Vector3(0.0, 0.0, 1.0), M_PI / 2.0),
                        tf::Vector3(1.0, 0.0, 0.5)),
          ros::Time(0),
          "odom", "map"),
      // rotate 90deg: (-3.0, 1.0, 0.0), r((0.0, 0.0, 1.0), M_PI)
      // offset x+1.0, z+0.5: t(-2.0, 1.0, 0.5), r((0.0, 0.0, 1.0), M_PI)
      tf::StampedTransform(
          tf::Transform(tf::Quaternion(tf::Vector3(0.0, 0.0, 1.0), M_PI),
                        tf::Vector3(-2.0, 1.0, 0.5)),
          ros::Time(0),
          "odom", "base_link"));

  test_transform(
      tf::StampedTransform(
          tf::Transform(tf::Quaternion(tf::Vector3(0.0, 0.0, 1.0), 0.0),
                        tf::Vector3(1.0, 1.0, 100.0)),
          ros::Time(0),
          "map", "base_link"),
      // projected: t(1.0, 1.0, 0.0), r((0.0, 0.0, 1.0), 0.0)
      tf::StampedTransform(
          tf::Transform(tf::Quaternion(tf::Vector3(1.0, 0.0, 0.0), M_PI / 6.0),
                        tf::Vector3(0.0, -sqrtf(3.0) / 2.0, 3.0 / 2.0)),
          ros::Time(0),
          "odom", "map"),
      // rotate 30deg and offset to make it on z axis
      tf::StampedTransform(
          tf::Transform(tf::Quaternion(tf::Vector3(1.0, 0.0, 0.0), M_PI / 6.0),
                        tf::Vector3(1.0, 0.0, 2.0)),
          ros::Time(0),
          "odom", "base_link"));
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
