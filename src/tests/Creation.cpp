/*
  _________                                 _________ .__            __    __                 __               
 /   _____/ ____  __ ______________ ___  __ \_   ___ \|  |__ _____ _/  |__/  |_  ___________ |__| ____   ____  
 \_____  \ /  _ \|  |  \_  __ \__  \\  \/ / /    \  \/|  |  \\__  \\   __\   __\/ __ \_  __ \|  |/ __ \_/ __ \ 
 /        (  <_> )  |  /|  | \// __ \\   /  \     \___|   Y  \/ __ \|  |  |  | \  ___/|  | \/|  \  ___/\  ___/ 
/_______  /\____/|____/ |__|  (____  /\_/    \______  /___|  (____  /__|  |__|  \___  >__/\__|  |\___  >\___  >
        \/                         \/               \/     \/     \/                \/   \______|    \/     \/ 

 * Copyright (c) 2024 Sourav Chatterjee
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Test.h"

TEST("Test can be created")
{
}
TEST_EX("Test with throw can be created", int)
{
    throw 1;
}

//This test should fail because it throws an unexpected exception
TEST("Test that throws unexpectedly can be created")
{
    setExpectedFailureReason("Unexpected exception thrown.");
    throw "Unexpected";
}

//This test should fail because it does not throw an exception that it is expecting to be thrown
TEST_EX("Test that never throws can be created", int)
{
    setExpectedFailureReason("Expected exception type int was not thrown.");
}

//This test should fail becuase it throws an exception that does not match the expected type
TEST_EX("Test that throws wrong type can be created", int)
{
    setExpectedFailureReason("Unexpected exception thrown.");
    throw "Wrong type";
}

//This test should throw an unexpected exception but it doesn't.
//We need to somehow let the user know what happened.
//This will result in a missed failure.
TEST("Test that should throw unexpectedly can be created")
{
    setExpectedFailureReason("Unexpected exception thrown.");
}