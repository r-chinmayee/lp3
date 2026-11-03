// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

contract StudentData {
    // a. Structure
    struct Student {
        uint256 id;
        string name;
        uint8 age;
    }

    // b. Array of structures
    Student[] public students;

    function addStudent(uint256 id, string memory name, uint8 age) public {
        students.push(Student(id, name, age));
    }

    function getStudent(uint256 index) public view returns (Student memory) {
        return students[index];
    }

    function totalStudents() public view returns (uint256) {
        return students.length;
    }

    // c. Fallback: runs when a call matches no function (or plain ETH is sent)
    fallback() external payable {}

    receive() external payable {}
}
