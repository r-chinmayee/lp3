// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

contract Bank {
    mapping(address => uint256) private balances;

    // a. Deposit money (send ETH along with the call)
    function deposit() public payable {
        require(msg.value > 0, "Deposit must be greater than 0");
        balances[msg.sender] += msg.value;
    }

    // b. Withdraw money (amount in wei)
    function withdraw(uint256 amount) public {
        require(balances[msg.sender] >= amount, "Insufficient balance");
        balances[msg.sender] -= amount;
        payable(msg.sender).transfer(amount);
    }

    // c. Show balance
    function showBalance() public view returns (uint256) {
        return balances[msg.sender];
    }
}